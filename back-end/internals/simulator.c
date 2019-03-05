#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

// this is the standard error function.
void error(char *error_msg) {
    printf("Error: %s\n", error_msg);
    exit(1);
}

// this function will extract all function declarations and include them in a .h
void format(char *file_name) {
    FILE *fp = fopen(file_name, "r");

    if(fp == NULL) {
        error("Unable to open the file.");
    }
    
    int n_characters = 0;
    int braces_stack = 0;
    char *first_level_code = (char*)malloc(1 * sizeof(char));
    first_level_code[0] = '\0';


    // first we want to extract all of the code not in functions.
    char c;
    while((c = fgetc(fp)) != EOF) {
        if(c == '{') {
            braces_stack++;
        } else if(c == '}') {
            braces_stack--;
        } else if(braces_stack == 0) {
            n_characters++;
            first_level_code = (char*)realloc(first_level_code, (n_characters + 1) * sizeof(char));

            if(c == '\n' || c == '\r') {
                c = ' ';
            }

            first_level_code[n_characters - 1] = c;
            first_level_code[n_characters] = '\0';
        }
    }    

    printf("%s\n", first_level_code);
    // now we need to filter the string to extract function names
    // functions can be extracted by searching for (...) 
    
    char **function_names;
    int n_functions = 0;
    int *function_lengths;

    // [0-9A-Za-z_\\[\\]\\*]+[[:space:]]+[0-9A-Za-z_\\[\\]\\*]+[[:space:]]*\\(.*\\)
    // a regex can be used to search for functions.
    char *function_pattern = "[0-9A-Za-z_\\[\\*]+]?[[:space:]]+[0-9A-Za-z_]+[[:space:]]*\\(.*\\)";
    int r_return;

    regex_t regex;
    if(regcomp(&regex, function_pattern, REG_EXTENDED)) {
        error("Could not compile regex.");
    }

    r_return = regexec(&regex, first_level_code, 0, NULL, 0);
    if(!r_return) {
        printf("Match\n");
    } else if(r_return == REG_NOMATCH) {
        printf("No match\n");
    }


    free(first_level_code);
    fclose(fp);
}

int main(int argc, char* argv[]) {
    char *CODE_FILE = "../user-code/valid_arduino.cpp";
    format(CODE_FILE);
}