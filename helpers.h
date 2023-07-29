#ifndef MAMAN_14_HELPERS_H
#define MAMAN_14_HELPERS_H

#include "globals.h"

#define SKIP_WHITE_SPACES(string, index) \
    while (string[(index)] && (string[(index)] == '\t' || string[(index)] == ' ')) { \
        ++(index); \
    }

int get_number_from_string(char *string, int *number);

int is_valid_label(const char *label);

int get_label_from_string(const char *string, char *label);

int is_empty_line(char *s);

int find_directive_by_name(char *line);

void remove_white_spaces(char *str, int start_index);

int is_valid_number(const char *str);



#endif
