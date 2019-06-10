#include <stdio.h>
#include <stdlib.h>
#include <cjson/cJSON.h>

#include "error.h"

// this is the standard error function. exits with code "return_code".
void error(char *error_msg, int return_code) {
    
    cJSON *root = cJSON_CreateObject();
    cJSON *msg = cJSON_CreateString(error_msg);
    cJSON_AddItemToObject(root, "msg", msg);
    cJSON_AddNumberToObject(root, "code", return_code);
    fprintf(stderr, "%s", cJSON_Print(root));
    cJSON_Delete(root);

    fflush(stderr);
    fflush(stdout);
    exit(return_code);
}