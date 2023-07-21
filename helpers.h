#ifndef MAMAN_14_HELPERS_H
#define MAMAN_14_HELPERS_H

int is_empty_line(char *s);

#define SKIP_WHITE_SPACES(string, index) \
    while (string[(index)] && (string[(index)] == '\t' || string[(index)] == ' ')) { \
        ++(index); \
    }



#endif
