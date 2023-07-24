#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "helpers.h"
#include "globals.h"

int is_empty_line(char *s) {
    while (*s != '\0') {
        if (!isspace(*s)) {
            return 0;
        }
        s++;
    }
    return 1;
}

void remove_white_spaces(char *str, int start_index) {
    int i = start_index;
    int j = 0;
    while (str[i] != '\0') {
        if (str[i] != ' ' && str[i] != '\t') {
            str[j] = str[i];
            ++j;
        }
        ++i;
    }
    str[j] = '\0';
}

void move_string_pointer_until_no_white_space(char **string) {
    while (**string && (**string == '\t' || **string == ' ')) {
        ++(*string);
    }
}

Directive directives[] = {
        {"data",   DATA},
        {"string", STRING},
        {"entry",  ENTRY},
        {"extern", EXTERN}
};


/**
 * @brief find the directive by name, gets a string that in the beginning of it there is a directive name
 * @param line
 * @return int
 */
int find_directive_by_name(char *line) {
    int i;
    for (i = 0; i < sizeof(directives) / sizeof(Directive); ++i) {
        if (strncmp(directives[i].name, line, strlen(directives[i].name)) == 0) {
            return directives[i].directive;
        }
    }
    return DIRECTIVE_NOT_FOUND;
}


Register registers[] = {
        {"r0", R0},
        {"r1", R1},
        {"r2", R2},
        {"r3", R3},
        {"r4", R4},
        {"r5", R5},
        {"r6", R6},
        {"r7", R7}
};

/**
 * @brief checks if the string is a valid number
 * @param line
 * @return int
 */
int is_valid_number(const char *str) {
    if (*str == '-' || *str == '+') {
        ++str;
    }
    while (*str) {
        if (!isdigit(*str)) {
            return 0;
        }
        ++str;
    }
    return 1;
}