#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "simulator.h"
#include "node.h"

void init(char *json) {

}

struct node * frame(struct node *in, struct process p) {
    // buff is current command (not nessesarily a full command)
    // can also be multiple commands and command fragments
    // write to fd on command.
    // should handle one command and adjust buff apropriately
    // returns new head of list (properly adjusted for command fragments)

    // as an example, we have a program where the process simply prints what it recvs and sends "hello process" to process
    // process code:

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