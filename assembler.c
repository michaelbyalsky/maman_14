#include "pre_assembler.h"
#include <string.h>
#include <stdio.h>

#include "first_run.h"
#include "code.h"
#include "tables.h"
#include "second_run.h"

int is_error = 0;
unsigned int line_address = INITIAL_ADDRESS;

static void process_file(char *filename);

int main(int argc, char *argv[]) {
    int i;
    for (i = 1; i < argc; ++i) {
        process_file(argv[i]);
    }
    return 0;
}

static void process_file(char *filename) {
    long ic = IC_START;
    long dc = DC_START;
    DataWord *dataImgHead = NULL;
    /* initialize the labelHead of the label list */
    Label *labelHead = NULL;
    CodeWord *codeHead = NULL;

    /* pre-assemble the file */
    char *outputFileName = pre_assemble(filename);
    is_error = 0;
    if (outputFileName == NULL) {
        return;
    }
    /* first run */
    first_run(outputFileName, &ic, &dc, &dataImgHead, &labelHead, &codeHead);

    if (is_error) {
        printf("Error in file %s\n", filename);
        return;
    }

    printLabelList(&labelHead);
    printCodeWordList(&codeHead);
    printDataWordList(&dataImgHead);

    printf("\n\n-------------Second run-------------\n\n");
    ic = IC_START;
    second_run(outputFileName, &ic, &dc, &dataImgHead, &labelHead, &codeHead);

    printLabelList(&labelHead);
    printCodeWordList(&codeHead);
    printDataWordList(&dataImgHead);

    freeLabelList(&labelHead);
    freeCodeWordList(&codeHead);
    freeDataWordList(&dataImgHead);
}