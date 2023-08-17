#ifndef MAMAN_14_FILE_HELPERS
#define MAMAN_14_FILE_HELPERS

#include "code.h"

/**
 * @brief write output files
 * @param filename - file name
 * @param codeHead - code list
 * @param dataImgHead - data image head
 * @param labelHead - label list
 * @param ic - instruction counter
 * @param dc - data counter
 */
void write_to_file(const char *filename, CodeWord **codeHead, DataWord **dataHead, Label **labelHead, unsigned int ic, unsigned int dc);

#endif
