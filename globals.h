#ifndef MAMAN_14_GLOBALS_H
#define MAMAN_14_GLOBALS_H

/* initial address for the program */
#define INITIAL_ADDRESS 100
/* max label size */
#define MAX_LABEL_SIZE 31
/* max line length */
#define MAX_LINE_LENGTH 81 /* include \n */
/* initial ic and dc */
#define IC_START 0
#define DC_START 0
/* error flag */
extern int is_error;
extern unsigned int line_address; /* address of the current line */

/* helper function result */
typedef struct FuncResult {
    int result;
    char *message[MAX_LINE_LENGTH];
} FuncResult;


#endif

