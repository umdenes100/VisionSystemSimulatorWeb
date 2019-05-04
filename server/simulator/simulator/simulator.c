#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <sys/prctl.h>
#include <signal.h>
#include <unistd.h>
#include <cjson/cJSON.h>

#include "compile.h"
#include "vs.h"
#include "simulator.h"
#include "node.h"

// this is the standard error function. exits with code 1.
void error(char *error_msg, int return_code) {
    fprintf(stderr, "Error: {\r\n\t'msg': '%s',\r\n\t'code': '%d'\r\n}\r\n", error_msg, return_code);
    fflush(stderr);
    fflush(stdout);
    exit(return_code);
}

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

    error("Unable to get id.", 8);
}

char* get_code(cJSON *json) {
    while(json != NULL) {
        if(!strcmp(json->string, "code")) {
            return json->valuestring;
        }

        json = json->next;
    }

    error("Unable to get code.", 7);
}

cJSON* get_randomization(cJSON *json) {
    while(json != NULL) {
        if(!strcmp(json->string, "randomization")) {
            return json->child;
        }

        json = json->next;
    }

    error("Unable to get randomization.", 6);
}

cJSON* clean_for_simulate(cJSON *json) {
    // get rid of type and code
    json = json->next->next;
    // keep randomization, distance sensors, id
    return json;
}

struct arena get_init(cJSON *json) {
    cJSON *randomization = json->child;
    cJSON *distance_sensors = json->next->child;

    randomization = randomization->next;
    cJSON *osv = randomization;
    cJSON *obstacles = randomization->next->child;
    cJSON *destination = randomization->next->next;

    struct arena arena;
    arena.destination.x = (float)destination->child->valuedouble;
    arena.destination.y = (float)destination->child->next->valuedouble;

    arena.osv.location.x = (float)osv->child->valuedouble;
    arena.osv.location.y = (float)osv->child->next->valuedouble;
    arena.osv.location.theta = (float)osv->child->next->next->valuedouble;
    arena.osv.height = (float)osv->child->next->next->next->valuedouble;
    arena.osv.width = (float)osv->child->next->next->next->next->valuedouble;
    arena.osv.left_motor_pwm = 0;
    arena.osv.right_motor_pwm = 0;

    while(distance_sensors != NULL) {
        arena.osv.distance_sensors[distance_sensors->valueint] = 1;
        distance_sensors = distance_sensors->next;
    }

    arena.obstacles = (struct obstacle *)malloc(1 * sizeof(struct obstacle));
    int num_obstacles = 0;
    cJSON *curr = obstacles;
    while(curr != NULL) {
        num_obstacles++;
        arena.obstacles = (struct obstacle *)realloc(arena.obstacles, num_obstacles * sizeof(struct obstacle));
        arena.obstacles[num_obstacles - 1].location.x = curr->child->valuedouble;
        arena.obstacles[num_obstacles - 1].location.y = curr->child->next->valuedouble;
        arena.obstacles[num_obstacles - 1].width = curr->child->next->next->valuedouble;
        arena.obstacles[num_obstacles - 1].height = curr->child->next->next->next->valuedouble;
        curr = curr->next;
    }

    arena.num_obstacles = num_obstacles;
    return arena;    
}

int ngets(char *new_buffer, int fd) {
    int size = read(fd, new_buffer, BUFFER_SIZE);
    if(size == -1) {
        return 0;
    } else {
        return size;
    }
}

struct process copen(char *command) {
    char *argv[] = { NULL };
    int in_pipe[2];
    int out_pipe[2];

    if(pipe(in_pipe) < 0) {
        error("Unable to pipe.", 3);
    }

    if(pipe(out_pipe) < 0) {
        error("Unable to pipe.", 3);
    }

    int pid = fork();
    switch(pid) {
        case -1:
        // this error
        error("Unable to fork.", 4);
        case 0:
        // this is child
        // we want stdout -> pipe, stdin -> pipe
        close(out_pipe[1]);
        close(in_pipe[0]);

        dup2(out_pipe[0], fileno(stdin));
        dup2(in_pipe[1], fileno(stdout));

        // ask kernel to deliver SIGTERM in case the parent dies
        prctl(PR_SET_PDEATHSIG, SIGTERM);

        // run the command
        if(execvp(command, argv) == -1) {
            error("An error occured in execvp", 5);
        }

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
    struct timespec t;
    clock_gettime(CLOCK_REALTIME, &t);
    return t.tv_sec;
}

unsigned long time_nsec() {
    struct timespec t;
    clock_gettime(CLOCK_REALTIME, &t);
    return t.tv_nsec;
}

int main(int argc, char *argv[]) {
    char *input = get_input();
    cJSON *json = cJSON_Parse(input);

    if(json == NULL) {
        error("Unable to parse JSON.", 1);
    }

    cJSON *parent_json = json;
    cJSON *child_json = json->child;

    // now that we have the JSON we need to perform initialization
    if(initialize(get_id(child_json), get_code(child_json)) != 0) {
        // initialize error:
        error("Unable to compile provided code.", 2);
    }

    child_json = clean_for_simulate(child_json);
    struct arena arena = get_init(child_json);

    // we have to run the processs
    char *command = (char*)malloc((strlen("./../environments//") + 2 * strlen(get_id(child_json))));
    sprintf(command, "./../environments/%s/%s", get_id(child_json), get_id(child_json));

    struct node *head = NULL;
    struct node *curr = head;

    unsigned long curr_nsec = time_nsec();
    int frame_no = 0;

    struct process p = copen(command);
    free(command);
    fcntl(p.input_fd, F_SETFL, O_NONBLOCK);

    printf("[");
    while(frame_no < NUM_FRAMES) {
        while(time_nsec() - curr_nsec < FRAME_RATE_NSEC);
        // This itteration happens each frame

        char *curr_buff = (char*)malloc(BUFFER_SIZE * sizeof(char));
        int curr_size = ngets(curr_buff, p.input_fd);
        if(curr_size == 0) {
            free(curr_buff);
        } else {
            // we want to create a linked list of packets (queue)
            // resize for space (?)
            curr_buff = (char*)realloc(curr_buff, curr_size * sizeof(char));
            if(head == NULL) {
                head = new_node(curr_buff, curr_size);
                curr = head;
            } else {
                curr->next = new_node(curr_buff, curr_size);
                curr = curr->next;
            }
        }

        head = frame(head, p, &arena, frame_no);
        curr_nsec = time_nsec();

        frame_no++;
    }
    printf("]");

    cclose(p);
    cJSON_Delete(parent_json);
    free(input);
    fflush(stdout);
    return 0;
}