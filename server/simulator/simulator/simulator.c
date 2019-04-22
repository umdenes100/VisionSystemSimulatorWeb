#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <cjson/cJSON.h>

#include "compile.h"

#define TIMEOUT 10000
#define BUFFER_SIZE 100
#define FRAME_RATE 10

// this function is a debugging function which creates a string of the arduino code
char* get_input() {
    char *string = (char*)malloc(1 * sizeof(char));
    int n_characters = 0;
    string[n_characters] = '\0';

    char c;

    while((c = getchar()) != EOF) {
        n_characters++;
        string = (char*)realloc(string, (n_characters + 1) * sizeof(char));
        string[n_characters] = '\0';
        string[n_characters - 1] = c;
    }

    return string;
}

char* get_id(cJSON *json) {
    while(json != NULL) {
        if(!strcmp(json->string, "id")) {
            return json->valuestring;
        }

        json = json->next;
    }

    error("Unable to get id.");
}

char* get_code(cJSON *json) {
    while(json != NULL) {
        if(!strcmp(json->string, "code")) {
            return json->valuestring;
        }

        json = json->next;
    }

    error("Unable to get code.");
}

cJSON* get_randomization(cJSON *json) {
    while(json != NULL) {
        if(!strcmp(json->string, "randomization")) {
            return json->child;
        }

        json = json->next;
    }

    error("Unable to get randomization.");
}

cJSON* clean_for_simulate(cJSON *json) {
    // get rid of type and code
    json = json->next->next;
    // keep randomization, distance sensors, id
    return json;
}

char ngetc(int fd) {
    char c;
    int size = read(fd, &c, 1);
    if(size == 0) {
        return (char)(-1);
    } else {
        return c;
    }
}

int ngets(char *new_buffer, int fd) {
    return read(fd, new_buffer, BUFFER_SIZE);
}

void frame(char **buff, int fd) {
    // buff is current command (not nessesarily a full command)
    // can also be multiple commands and command fragments
    // write to fd on command.
    // should handle one command and adjust buff apropriately
}

int main(int argc, char *argv[]) {
    char *input = get_input();
    cJSON *json = cJSON_Parse(input);

    if(json == NULL) {
        fprintf(stderr, "Unable to parse JSON.\n");
        fflush(stderr);
        return -1;
    }

    cJSON *parent_json = json;
    cJSON *child_json = json->child;

    // now that we have the JSON we need to perform initialization
    if(initialize(get_id(child_json), get_code(child_json)) != 0) {
        // initialize error:
        return -1;
    }

    child_json = clean_for_simulate(child_json);
    parent_json->child = child_json;

    // we have to run the processs
    char *json_output = cJSON_Print(parent_json);
    char *program_input = (char*)malloc((strlen(json_output) + 2) * sizeof(char));
    sprintf(program_input, "'%s'", json_output);
    char *command = (char*)malloc((strlen(program_input) + strlen("./../environments//") + 2 * strlen(get_id(child_json)) + strlen(" ")));
    sprintf(command, "./../environments/%s/%s %s", get_id(child_json), get_id(child_json), program_input);
    
    FILE* p = popen(command, "r");
    if(!p) {
        return -1;
    }
    
    int fd = fileno(p);
    // we need a non-blocking read this is called ngetc
    fcntl(fd, F_SETFL, O_NONBLOCK);

    char *buff = (char*)malloc(1 * sizeof(char));
    int size = 0;
    buff[size] = '\0';
    char *curr_buff = (char*)malloc(BUFFER_SIZE * sizeof(char));
    time_t start = time(NULL);
    time_t last = time(NULL);

    while(last - start > TIMEOUT) {
        while(time(NULL) - last < FRAME_RATE);
        // This itteration happens each frame
        int curr_size = ngets(curr_buff, fd);
        buff = (char*)realloc(buff, (curr_size + size + 1) * (sizeof(char)));
        strcat(buff, curr_buff);
        frame(&buff, fd);
        last = time(NULL);
    }

    pclose(p);
    free(buff);
    free(curr_buff);

    // now we need to simulate
    cJSON_Delete(parent_json);
    free(buff);
    free(input);

    fflush(stdout);
    return 0;
}