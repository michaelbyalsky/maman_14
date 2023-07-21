#ifndef MAMAN_14_HELPERS_H
#define MAMAN_14_HELPERS_H

int is_empty_line(char *s);
int find_directive_by_name(char *line);

#define SKIP_WHITE_SPACES(string, index) \
    while (string[(index)] && (string[(index)] == '\t' || string[(index)] == ' ')) { \
        ++(index); \
    }



#endif
