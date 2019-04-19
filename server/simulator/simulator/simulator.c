#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cjson/cJSON.h>

#include "compile.h"

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

    char *json_output = cJSON_Print(parent_json);
    char *program_input = (char*)malloc((strlen(json_output) + 2) * sizeof(char));
    sprintf(program_input, "'%s'", json_output);
    char *command = (char*)malloc((strlen(program_input) + strlen("./../environments//") + 2 * strlen(get_id(child_json)) + strlen(" ")));
    sprintf(command, "./../environments/%s/%s %s", get_id(child_json), get_id(child_json), program_input);
    
    FILE* p = popen(command, "r");
    if(!p) {
        error("Could not read file.");
    }

    char *buff = (char*)malloc(1 * sizeof(char));
    int i = 0;
    while((buff[i] = fgetc(p)) != EOF) {
        i++;
        buff = realloc(buff, (i + 1) * sizeof(char));
    }

    buff[i] = '\0';
    pclose(p);
    printf("%s\n", buff);

    // now we need to simulate
    cJSON_Delete(parent_json);
    free(buff);
    free(input);

    fflush(stdout);
    return 0;
}