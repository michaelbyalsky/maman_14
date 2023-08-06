#ifndef MAMAN_14_GLOBALS_H
#define MAMAN_14_GLOBALS_H


#define INITIAL_ADDRESS 100
#define MAX_LABEL_SIZE 31
#define MAX_LINE_LENGTH 81 /* include \n */
#define INITIAL_ADDRESS 100
#define IC_START 0
#define DC_START 0
extern int is_error; /* flag for file */
extern int line_address; /* address of the current line */

typedef struct FuncResult {
    int result;
    char *message[MAX_LINE_LENGTH];
} FuncResult;


#endif

