#ifndef COMPILE_H
#define COMPILE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <sys/stat.h>

struct match_list {
    char **matches;
    int n_matches;
};

struct file_names {
    char *hdr;
    char *src;
};

int initialize(char *program_name, char *code);
void error(char *error_msg);

#endif