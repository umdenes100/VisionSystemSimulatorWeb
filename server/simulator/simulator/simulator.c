#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <fcntl.h>
#include <sys/prctl.h>
#include <signal.h>
#include <unistd.h>
#include <cjson/cJSON.h>

#include "compile.h"

#define TIMEOUT_SEC 1
#define BUFFER_SIZE 100
#define FRAME_RATE_NSEC 20000

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

struct process {
    int pid;
    int input_fd;
    int output_fd;
};

void frame(char **buff, struct process p) {
    // buff is current command (not nessesarily a full command)
    // can also be multiple commands and command fragments
    // write to fd on command.
    // should handle one command and adjust buff apropriately
}

struct process copen(char *command) {
    char *argv[] = { command, NULL };
    int in_pipe[2];
    int out_pipe[2];
    if(pipe(in_pipe) < 0) {
        error("Unable to pipe.");
    }

    if(pipe(out_pipe) < 0) {
        error("Unable to pipe.");
    }

    int pid = fork();
    switch(pid) {
        case -1:
        // this error
        error("Unable to fork.");
        case 0:
        // this is child
        // we want stdout -> pipe, stdin -> pipe
        close(out_pipe[1]);
        close(in_pipe[0]);

        dup2(out_pipe[0], STDIN_FILENO);
        dup2(in_pipe[1], STDOUT_FILENO);

        //ask kernel to deliver SIGTERM in case the parent dies
        prctl(PR_SET_PDEATHSIG, SIGTERM);

        // run the command
        execvp(command, argv);
        exit(0);
        break;
        default:
        // this is parent with child pid
        close(out_pipe[0]);
        close(in_pipe[1]);
        break;
    }

    struct process p;
    p.pid = pid;
    p.input_fd = in_pipe[0];
    p.output_fd = out_pipe[1];

    return p;
}

void cclose(struct process p) {
    kill(p.pid, SIGKILL);
}

unsigned long time_sec() {
    struct timeval t;
    gettimeofday(&t, NULL);
    return t.tv_sec;
}

unsigned long time_nsec() {
    struct timeval t;
    gettimeofday(&t, NULL);
    return t.tv_nsec;
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
    
    struct process p = copen(command);
    // we need a non-blocking read this is called ngetc
    fcntl(p.input_fd, F_SETFL, O_NONBLOCK);

    char *buff = (char*)malloc(1 * sizeof(char));
    int size = 0;
    buff[size] = '\0';
    char *curr_buff = (char*)malloc(BUFFER_SIZE * sizeof(char));

    unsigned long start = time_sec();
    unsigned long curr_sec = time_sec();
    unsigned long curr_nsec = time_nsec();

    while(curr_sec - start < TIMEOUT_SEC) {
        printf("Hello\n");
        while(time_nsec() - curr_nsec < FRAME_RATE_NSEC);
        // This itteration happens each frame
        int curr_size = ngets(curr_buff, p.input_fd);
        // buff = (char*)realloc(buff, (curr_size + size + 1) * (sizeof(char)));
        strcat(buff, curr_buff);
        frame(&buff, p);

        curr_sec = time_sec();
        curr_nsec = time_nsec();
    }

    cclose(p);
    free(buff);
    free(curr_buff);

    // now we need to simulate
    cJSON_Delete(parent_json);
    free(input);

    fflush(stdout);
    return 0;
}