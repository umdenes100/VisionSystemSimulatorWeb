#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <cjson/cJSON.h>

#include "simulator.h"
#include "node.h"
#include "vs.h"

void init(char *json) {

}

struct node * frame(struct node *in, struct process p, struct arena *arena) {
    // in is a linked list with the current command in parts
    // the entire list will always contain a total of one commands or less

    /*
     * #include <stdio.h>
     * #include <stdlib.h>
     *
     * void setup() {
     *     printf("1");
     *     fflush(stdout);
     * }
     * 
     * void loop() {
     *     char buff[14];
     *     if(fgets(buff, 14, stdin) != NULL) {
     *         printf("got '%s', sending now...", buff);
     *         fflush(stdout);
     *     }
     * }
     * 
    */

    if(in != NULL) {
        printf("recv'd: '%s'\n", in->data);
        write(p.output_fd, "hello process", strlen("hello process"));
        in = in->next;
    }


    static int frame_no = 0;
    cJSON *root = cJSON_CreateObject();
    cJSON *osv = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "frame_no", frame_no);
    cJSON_AddNumberToObject(osv, "x", arena->osv.location.x);
    cJSON_AddNumberToObject(osv, "y", arena->osv.location.y);
    cJSON_AddNumberToObject(osv, "theta", arena->osv.location.theta);
    cJSON_AddItemToObject(root, "osv", osv);
    frame_no++;

    printf("%s,", cJSON_Print(root));
    cJSON_Delete(root); 

    return in;
}