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

int second_run(char *filename, long *ic, long *dc, DataWord *data_img, Label **labelHead, CodeWord **codeHead);

#endif
