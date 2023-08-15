#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "globals.h"
#include "helpers.h"
#include "tables.h"
#include "code.h"

#ifndef MAMAN_14_SECOND_RUN_H
#define MAMAN_14_SECOND_RUN_H

/**
 * @brief process single line second run
 * @param filename - the file name
 * @param ic - instruction counter
 * @param labelHead - label list
 * @param codeHead - code list
 * @return - int
 */
int second_run(char *filename, unsigned int *ic, Label **labelHead, CodeWord **codeHead);

#endif
