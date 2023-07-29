#ifndef MAMAN_14_FIRST_RUN_H
#define MAMAN_14_FIRST_RUN_H

extern int is_label; /* flag for label in the line */
extern int line_number; /* line number in the file */

int first_run(char *filename, long *ic, long *dc, DataWord *data_img, Label **head, CodeWord **codeHead);


#endif
