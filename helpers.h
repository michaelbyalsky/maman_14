#ifndef MAMAN_14_HELPERS_H
#define MAMAN_14_HELPERS_H

#include "globals.h"

int is_empty_line(char *s);
int find_directive_by_name(char *line);
void remove_white_spaces(char *str, int start_index);
int is_valid_number(const char *str);
Label *createLabelNode(const char *name, long address);
void insertLabelNode(Label **head, const char *name, long address);
void printLabelList(Label **head);

#define SKIP_WHITE_SPACES(string, index) \
    while (string[(index)] && (string[(index)] == '\t' || string[(index)] == ' ')) { \
        ++(index); \
    }



#endif
