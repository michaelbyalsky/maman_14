#include <string.h>
#include <stdio.h>

#include "pre_assembler.h"
#include "first_run.h"
#include "code.h"
#include "tables.h"
#include "second_run.h"
#include "file_helpers.h"

int is_error = 0;
unsigned int line_address = INITIAL_ADDRESS;

void process_file(char *filename);

int main(int argc, char *argv[]) {
    int i;
    for (i = 1; i < argc; ++i) {
        process_file(argv[i]);
    }
    return 0;
}

/**
 * @brief processes single file
 * @param filename
 */
void process_file(char *filename) {
    unsigned int ic = IC_START;
    unsigned int dc = DC_START;
    /* initialize the data image table */
    DataWord *dataImgHead = NULL;
    /* initialize the label table */
    Label *labelHead = NULL;
    /* initialize the code table */
    CodeWord *codeHead = NULL;
    char *outputFileName;
    /* validate file ends with .as */
    if (!validate_file_ends_with_as(filename)) {
        return;
    }

    /* pre-assemble the file */
    outputFileName = pre_assemble(filename);

    line_address = INITIAL_ADDRESS;
    is_error = 0;
    if (outputFileName == NULL) {
        return;
    }
    /* first run */
    first_run(outputFileName, &ic, &dc, &dataImgHead, &labelHead, &codeHead);

    if (is_error) {
        printf("Error in first pass, file %s\n", outputFileName);
        return;
    }

    ic = IC_START;
    line_address = INITIAL_ADDRESS;

    /* second run */
    second_run(outputFileName, &ic, &labelHead, &codeHead);
    if (is_error) {
        printf("Error in second pass, file %s\n", outputFileName);
        return;
    }

    free(outputFileName);

    write_to_file(filename, &codeHead, &dataImgHead, &labelHead, ic, dc);

    free_label_list(&labelHead);
    free_code_word_list(&codeHead);
    free_data_word_list(&dataImgHead);
}
