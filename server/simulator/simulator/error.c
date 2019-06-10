#include <stdio.h>
#include <stdlib.h>

#include "error.h"

// this is the standard error function. exits with code "return_code".
void error(char *error_msg, int return_code) {
    fprintf(stderr, "Error: {\r\n\t'msg': '%s',\r\n\t'code': '%d'\r\n}\r\n", error_msg, return_code);
    fflush(stderr);
    fflush(stdout);
    exit(return_code);
}