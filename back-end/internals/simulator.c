#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

// struct to return char[][] of matches
struct match_list {
    char **matches;
    int n_matches;
};

// this is the standard error function.
void error(char *error_msg) {
    printf("Error: %s\n", error_msg);
    exit(1);
}

// this function will retrieve all matches to the regex in the string
struct match_list get_all_matches(regex_t r, char *to_match) {
    char **matches = (char**)malloc(1 * sizeof(char*));
    int n_matches = 0;

    while(1) {
        regmatch_t details;

        if(!regexec(&r, to_match, (size_t) 1, &details, 0)) {
            // we have a match, now we need to extract the match
            n_matches++;
            matches = (char**)realloc(matches, n_matches * sizeof(char*));
            matches[n_matches - 1] = (char*)malloc((details.rm_eo - details.rm_so + 1) * sizeof(char));
            int i;
            for(i = details.rm_so; i < details.rm_eo; i++) {
                matches[n_matches - 1][i - details.rm_so] = to_match[i];
            }

            matches[n_matches - 1][i - details.rm_so] = '\0';
            
            // now we want to move on to next possible match
            to_match += details.rm_eo;
        } else {
            break;
        }
    }

    // here we have all possible matches.
    struct match_list ret;
    ret.matches = matches;
    ret.n_matches = n_matches;

    return ret;
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

            if(c == ')') {
                // we need to add this to help with the regex (c has a weird regex matching rule)
                n_characters++;
                first_level_code = (char*)realloc(first_level_code, (n_characters + 1) * sizeof(char));
                first_level_code[n_characters - 1] = '#';
                first_level_code[n_characters] = '\0';
            }
        }
    }    

    // printf("%s\n", first_level_code);
    // now we need to filter the string to extract function names
    // functions can be extracted by searching for (...) 

    // [0-9A-Za-z_\\[\\]\\*]+[[:space:]]+[0-9A-Za-z_\\[\\]\\*]+[[:space:]]*\\(.*\\)
    // a regex can be used to search for functions.
    char *function_pattern = "[0-9A-Za-z_\\[\\*]+]?[[:space:]]+[0-9A-Za-z_]+[[:space:]]*\\([^#]*\\)";
    int r_return;

    regex_t regex;
    if(regcomp(&regex, function_pattern, REG_EXTENDED)) {
        error("Could not compile regex.");
    }

    struct match_list matches = get_all_matches(regex, first_level_code);
    // printf("%d\n", matches.n_matches);

    free(first_level_code);
    fclose(fp);

    // now we have a list of all function prototypes, we have to add the prototypes to the code
    // we can do this in the form of a header file
    char header_file[strlen(file_name) - 2];
    int i;
    for(i = 0; i < strlen(file_name) - 2; i++) {
        header_file[i] = file_name[i];
    }

    header_file[i - 1] = 'h';
    header_file[i] = '\0';

    // printf("%s\n", header_file);
    fp = fopen(header_file, "w");
    fputs("#ifndef PROTOTYPES_H\n#define PROTOTYPES_H\n\n", fp);
    
    for(i = 0; i < matches.n_matches; i++) {
        fputs(matches.matches[i], fp);
        fputs(";\n", fp);
    }

    fputs("\n#endif", fp);

    fclose(fp);

    // now we have taken care of function prototypes, we now need to take care of main function.
    fp = fopen(file_name, "a");
    fputs("\n\nint main() {\n\tsetup();\n\twhile(1) {\n\t\tloop();\n\t}\n}\n", fp);

    for(i = 0; i < matches.n_matches; i++) {
        free(matches.matches[i]);
    }

    free(matches.matches);

    fclose(fp);
}

int main(int argc, char* argv[]) {
    char *CODE_FILE = "../user-code/valid_arduino.cpp";
    format(CODE_FILE);
}