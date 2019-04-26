#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "simulator.h"

void init() {

}

void frame(char **buff, struct process p) {
    // buff is current command (not nessesarily a full command)
    // can also be multiple commands and command fragments
    // write to fd on command.
    // should handle one command and adjust buff apropriately
}