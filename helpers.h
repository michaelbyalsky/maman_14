#ifndef MAMAN_14_HELPERS_H
#define MAMAN_14_HELPERS_H

int is_empty_line(char *s);
int find_directive_by_name(char *line);
void remove_white_spaces(char *str, int start_index);
int is_valid_number(const char *str);

#define MAX_INT 2147483647
#define MIN_INT -2147483648
#define SKIP_WHITE_SPACES(string, index) \
    while (string[(index)] && (string[(index)] == '\t' || string[(index)] == ' ')) { \
        ++(index); \
    }



#endif
