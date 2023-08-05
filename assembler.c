#include "pre_assembler.h"
#include <string.h>
#include <stdio.h>

#include "first_run.h"
#include "code.h"
#include "tables.h"

int is_error = 0;

static void process_file(char *filename);

int main(int argc, char *argv[]) {
    int i;
    for (i = 1; i < argc; ++i) {
        process_file(argv[i]);
    }
    return 0;
}

static void process_file(char *filename) {
    int i;
    long ic = IC_START;
    long dc = DC_START;
    DataWord data_img[CODE_IMG_LENGTH];
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
    first_run(outputFileName, &ic, &dc, data_img, &labelHead, &codeHead);

    /* print the data image */
    for (i = 0; i < dc; ++i) {
        if (data_img[i].datatype == DATA) {
            printf("%d: %d\n", i, data_img[i].NumberStringUnion.number);
        } else if (data_img[i].datatype == STRING) {
            printf("%d: %s\n", i, data_img[i].NumberStringUnion.string);
        }
    }

    printf("ic: %ld\n", ic);

    /* print the label list */
    printLabelList(&labelHead);
    freeLabelList(&labelHead);

    printCodeWordList(&codeHead);
    freeCodeWordList(&codeHead);
}