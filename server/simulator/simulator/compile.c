#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <sys/stat.h>

#include "compile.h"
#include "error.h"

// this function will retrieve all matches to the regex in the string
struct match_list get_all_matches(regex_t r, char *to_match) {
    char **matches = (char**)malloc(1 * sizeof(char*));  //why multiply by 1? also need to figure out this pointer math
    int n_matches = 0;

    while(1) {
        regmatch_t details; //records the matches of the regex, the first element records the part of the string that matches the entire element

        if(!regexec(&r, to_match, (size_t) 1, &details, 0)) {
            // we have a match, now we need to extract the match
            n_matches++;
            matches = (char**)realloc(matches, n_matches * sizeof(char*)); //reallocating memory
            //subtract the offset of the beginning and end of the match and then we get the number of characters
            matches[n_matches - 1] = (char*)malloc((details.rm_eo - details.rm_so + 1) * sizeof(char)); 
            int i;
            for(i = details.rm_so; i < details.rm_eo; i++) {
                matches[n_matches - 1][i - details.rm_so] = to_match[i]; //seems like we are adding the match to array
            }

            matches[n_matches - 1][i - details.rm_so] = '\0'; //null termination
            
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
struct match_list get_function_declarations(char *file_name, int *status_code) {
    FILE *fp = fopen(file_name, "r");

    struct match_list dummy;

    if(fp == NULL) {
        error("Unable to read file.", 2);
        *status_code = -1;
        return dummy;
    }

    int n_characters = 0;
    int braces_stack = 0;
    char *first_level_code = (char*)malloc(1 * sizeof(char)); //initial pointer
    first_level_code[0] = '\0'; //null


    // first we want to extract all of the code not in functions.
    char c;
    while((c = fgetc(fp)) != EOF) {
        if(c == '{') {
            braces_stack++;
        } else if(c == '}') {
            braces_stack--;
        } else if(braces_stack == 0) {
            n_characters++;
            //reallocate first_level_code with the number of characters
            first_level_code = (char*)realloc(first_level_code, (n_characters + 1) * sizeof(char)); 

            //guessign this is to prevent shananigens involved once we put 'c' into array
            if(c == '\n' || c == '\r') {
                c = ' ';
            }

            first_level_code[n_characters - 1] = c;
            first_level_code[n_characters] = '\0'; // end is null terminated

            if(c == ')') {
                // we need to add this to help with the regex (c has a weird regex matching rule)
                n_characters++;
                first_level_code = (char*)realloc(first_level_code, (n_characters + 1) * sizeof(char));
                first_level_code[n_characters - 1] = '`';
                first_level_code[n_characters] = '\0';
            }
        }
    }    

    // printf("%s\n", first_level_code);
    // now we need to filter the string to extract function names
    // functions can be extracted by searching for (...) 

    // [0-9A-Za-z_\\[\\]\\*]+[[:space:]]+[0-9A-Za-z_\\[\\]\\*]+[[:space:]]*\\(.*\\)
    // a regex can be used to search for functions.
    char *function_pattern = "[0-9A-Za-z_\\[\\*]+]?[[:space:]]+[0-9A-Za-z_]+[[:space:]]*\\([^`]*\\)";
    int r_return;

    regex_t regex;
    if(regcomp(&regex, function_pattern, REG_EXTENDED)) {
        error("Unable to compile regular expression.", 2);
        *status_code = -1;
        return dummy;
    }

    struct match_list matches = get_all_matches(regex, first_level_code);
    // printf("%d\n", matches.n_matches);

    free(first_level_code); 
    fclose(fp);

    return matches;
}

// this function creates a directory for the program using the program name
int create_dir(char *name) {
    //is 17 connected to the "../environments/" ?
    char dest[17 + strlen(name) + 1];

    int i;
    for(i = 0; i < 17; i++) {
        dest[i] = "../environments/"[i];
    }

    strcat(dest, name);

    if(mkdir(dest, 0777) != 0) {
        error("Unable to create environment.", 2);
        return -1;
    }

    return 0;
}

// this function returns the .cpp and .h file paths for the program
struct file_names get_file_names(char *file_name) {
    int s_len = strlen("../environments/");
    int f_len = strlen(file_name);
    int src_len = s_len + 2 * f_len + 1 + strlen(".cpp"); //what's the point of 2*f_len?
    int hdr_len = s_len + 2 * f_len + 1 + strlen(".h");

    char *src = (char*)malloc((src_len + 1) * sizeof(char)); 
    char *hdr = (char*)malloc((hdr_len + 1) * sizeof(char));

    int i;
    for(i = 0; i < s_len; i++) {
        src[i] = "../environments/"[i];
        hdr[i] = "../environments/"[i];
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
int create_src_file(char *code, struct file_names files) {
    FILE *fp = fopen(files.src, "w");

    if(fp == NULL) {
        error("Unable to create src file.", 2);
        return -1;
    }
    //we are writing the contents below into the src file
    char n_line[strlen("#include ''") + strlen(files.hdr) + 2];
    n_line[0] = '\0';
    strcat(n_line, "#include \"");
    strcat(n_line, files.hdr);
    strcat(n_line, "\"\n");

    fputs(n_line, fp);
    fputs(code, fp);
    fputs("\n\nint main(int argc, char *argv[]) {\n\tsetup();\n\twhile(1) {\n\t\tloop();\n\t}\n}\n", fp);
    fclose(fp);

    return 0;
}

// this function will create the header file
int create_hdr_file(struct match_list functions, char *file) {
    FILE *fp = fopen(file, "w");

    if(fp == NULL) {
        error("Unable to create hdr file.", 2);
        return -1;
    }

    fputs("#include \"VisionSystemClient.hpp\"\n\n#ifndef PROTOTYPES_H\n#define PROTOTYPES_H\n\n", fp);
    
    int i;
    for(i = 0; i < functions.n_matches; i++) {
        fputs(functions.matches[i], fp);
        fputs(";\n", fp);
    }

    fputs("\n#endif", fp);

    fclose(fp);

    return 0;
}

// this function frees a match_set
void free_match_list(struct match_list m) {
    int i;

    for(i = 0; i < m.n_matches; i++) {
        free(m.matches[i]);
    }

    free(m.matches);
}

// this is the final function to compile the executable
int compile(char *file) {
    int len_base = strlen("cd ../dependencies & make name=");
    int len_f = strlen(file);
    int len = len_f + len_base + strlen(" 2>&1");
    char command[len + 1];


    int i;
    for(i = 0; i < len_base; i++) {
        command[i] = "cd ../dependencies ; make name="[i];
        command[i + 1] = '\0';
    }

    strcat(command, file);
    strcat(command, " 2>&1");

    FILE* p = popen(command, "r");
    if(!p) {
        error("Unable to open gcc command.", 2);
        return -1;
    }

    char *buff = (char*)malloc(1 * sizeof(char));
    i = 0;
    while((buff[i] = fgetc(p)) != EOF) {
        i++;
        buff = realloc(buff, (i + 1) * sizeof(char));
    }

    buff[i] = '\0';
    int code = pclose(p);

    if(code != 0) {
        error(buff, 2);
        return -1;
    }

    return 0;
}

int initialize(char *program_name, char *code) {
    // first we need to create the environment

    // first we create the folder we will store info in
    if(create_dir(program_name) != 0) {
        return -1;
    }

    // its convinient not to get all of the names of the files we're using
    struct file_names files = get_file_names(program_name);
    
    // now we want to write the src into persistent memory with a main function
    if(create_src_file(code, files) != 0) {
        return -1;
    }

    // now we need to get the function declarations for the .h file
    int status_code = 0;
    struct match_list functions = get_function_declarations(files.src, &status_code);

    if(status_code != 0) {
        return -1;
    }

    // now we want to write the .h file
    if(create_hdr_file(functions, files.hdr) != 0) {
        return -1;
    }

    free_match_list(functions);
    
    if(compile(program_name) != 0) {
        return -1;
    }

    free(files.src);
    free(files.hdr);

    return 0;
}

int cleanup(char *program_name) {
    char dest[17 + strlen(program_name) + 1];

    int i;
    for(i = 0; i < 17; i++) {
        dest[i] = "../environments/"[i];
    }

    strcat(dest, program_name);

    char command[17 + strlen(program_name) + strlen("rm -rf") + strlen("2>&1") + 3];
    sprintf(command, "%s %s %s", "rm -rf", dest, "2>&1");

    FILE* p = popen(command, "r");
    
    if(!p || pclose(p)) {
        error("Unable to cleanup.", 2);
        return -1;
    }
}