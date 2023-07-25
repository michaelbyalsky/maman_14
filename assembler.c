#include "pre_assembler.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "first_run.h"
#include "helpers.h"


static int process_file(char *filename);

int main(int argc, char *argv[]) {
    int i;
    for (i = 1; i < argc; ++i) {
        process_file(argv[i]);
    }
    return 0;
}

static int process_file(char *filename) {
    long ic = IC_START;
    long dc = DC_START;
    DataWord data_img[CODE_IMG_LENGTH];
    /* initialize the head of the label list */
    Label *head = NULL;

    /* pre-assemble the file */
    char *outputFileName = pre_assemble(filename);
    if (outputFileName == NULL) {
        return -1;
    }

    /* first run */
    first_run(outputFileName, &ic, &dc, data_img, &head);
    /* print dc */
    printf("DC: %lu\n", dc);
    /* print the data image */
    printf("Data image:\n");
    int i;
    for ( i = 0; i < dc; ++i) {
        if (data_img[i].datatype == STRING) {
            printf("%lu: %c\n", dc, data_img[i].string);
        } else {
            printf("%lu: %d\n", dc, data_img[i].number);
        }
    }
    /* print the label list */
    printf("Label list:\n");
    printLabelList(head);
}