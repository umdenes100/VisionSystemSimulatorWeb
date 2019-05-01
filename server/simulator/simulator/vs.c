#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "simulator.h"
#include "node.h"

void init(char *json) {

}

struct node * frame(struct node *in, struct process p) {
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

    return in;
}