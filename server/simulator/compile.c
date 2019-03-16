#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <sys/stat.h>

// struct to return char[][] of matches
struct match_list {
    char **matches;
    int n_matches;
};

struct file_names {
    char *hdr;
    char *src;
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

// this function will get all of the function names
struct match_list get_function_declarations(char *file_name) {
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

    return matches;
}

// this function creates a directory for the program using the program name
void create_dir(char *name) {
    char dest[14 + strlen(name) + 1];

    int i;
    for(i = 0; i < 14; i++) {
        dest[i] = "../user-code/"[i];
    }

    strcat(dest, name);

    if(mkdir(dest, 0777) != 0) {
        error("Unable to create directory.");
    }
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

// this function returns the .cpp and .h file paths for the program
struct file_names get_file_names(char *file_name) {
    int s_len = strlen("../user-code/");
    int f_len = strlen(file_name);
    int src_len = s_len + 2 * f_len + 1 + strlen(".cpp");
    int hdr_len = s_len + 2 * f_len + 1 + strlen(".h");

    char *src = (char*)malloc((src_len + 1) * sizeof(char));
    char *hdr = (char*)malloc((hdr_len + 1) * sizeof(char));

    int i;
    for(i = 0; i < s_len; i++) {
        src[i] = "../user-code/"[i];
        hdr[i] = "../user-code/"[i];
    }

    for(i = 0; i < f_len; i++) {
        src[i + s_len] = file_name[i];
        hdr[i + s_len] = file_name[i];
    }

    src[s_len + f_len] = '/';
    hdr[s_len + f_len] = '/';

    for(i = 0; i < f_len; i++) {
        src[i + s_len + f_len + 1] = file_name[i];
        hdr[i + s_len + f_len + 1] = file_name[i];
    }

    src[s_len + 2 * f_len + 1] = '.';
    src[s_len + 2 * f_len + 2] = 'c';
    src[s_len + 2 * f_len + 3] = 'p';
    src[s_len + 2 * f_len + 4] = 'p';
    src[s_len + 2 * f_len + 5] = '\0';
    
    hdr[s_len + 2 * f_len + 1] = '.';
    hdr[s_len + 2 * f_len + 2] = 'h';
    hdr[s_len + 2 * f_len + 3] = '\0';
    
    struct file_names f;
    f.hdr = hdr;
    f.src = src;

    return f;
}

// this function will create a temp src file
void create_src_file(char *code, char *file) {
    FILE *fp = fopen(file, "w");

    if(fp == NULL) {
        error("Unable to create src file.");
    }

    fputs(code, fp);
    fputs("\n\nint main() {\n\tsetup();\n\twhile(1) {\n\t\tloop();\n\t}\n}\n", fp);
    fclose(fp);
}

// this function will create the header file
void create_hdr_file(struct match_list functions, char *file) {
    FILE *fp = fopen(file, "w");

    if(fp == NULL) {
        error("Unable to create header file.");
    }

    fputs("#ifndef PROTOTYPES_H\n#define PROTOTYPES_H\n\n", fp);
    
    int i;
    for(i = 0; i < functions.n_matches; i++) {
        fputs(functions.matches[i], fp);
        fputs(";\n", fp);
    }

    fputs("\n#endif", fp);

    fclose(fp);
}

// this function frees a match_set
void free_match_list(struct match_list m) {
    int i;

    for(i = 0; i < m.n_matches; i++) {
        free(m.matches[i]);
    }

    free(m.matches);
}

int main(int argc, char* argv[]) {
    // first we need to create the environment
    char *CODE_FILE = "valid_arduino";
    char *input = get_input();

    // first we create the folder we will store info in
    create_dir(CODE_FILE);

    // its convinient not to get all of the names of the files we're using
    struct file_names files = get_file_names(CODE_FILE);
    
    // now we want to write the src into persistent memory with a main function
    create_src_file(input, files.src);
    free(input);

    // now we need to get the function declarations for the .h file
    struct match_list functions = get_function_declarations(files.src);

    // now we want to write the .h file
    create_hdr_file(functions, files.hdr);

    free_match_list(functions);
    free(files.src);
    free(files.hdr);

    // now we have a .cpp, .h, and other libraries. we just need to compile the program.
}