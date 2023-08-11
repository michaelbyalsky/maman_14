#ifndef MAMAN_14_FIRST_RUN_H
#define MAMAN_14_FIRST_RUN_H

#include "code.h"

/**
 * @brief process single line first run
 * @param line - the line to process
 * @param ic - instruction counter
 * @param dc - data counter
 * @param dataImgHead - data image head
 * @param labelHead - label list
 * @param codeHead - code list
 */
void first_run(char *filename, long *ic, long *dc, DataWord **dataImgHead, Label **head, CodeWord **codeHead);

#endif
