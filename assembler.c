#include "pre_assembler.h"
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>

#include "first_run.h"
#include "code.h"
#include "tables.h"
#include "second_run.h"

char filePath[100];

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


/**
 * @brief processes single file
 * @param filename
 */
static void process_file(char *filename) {
    const char *entriesSuffix = ".ent";
    const char *externalsSuffix = ".ext";
    const char *objectSuffix = ".ob";
    char *entriesFileName;
    char *externalsFileName;
    char *objectFileName;
    FILE *entries;
    FILE *externals;
    FILE *object;
    long ic = IC_START;
    long dc = DC_START;
    /* initialize the data image table */
    DataWord *dataImgHead = NULL;
    /* initialize the label table */
    Label *labelHead = NULL;
    /* initialize the code table */
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
        printf("Error in first pass, file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    print_label_list(&labelHead);
    print_code_word_list(&codeHead);
    print_data_word_list(&dataImgHead);

    printf("\n\n-------------Second run-------------\n\n");
    ic = IC_START;
    line_address = INITIAL_ADDRESS;

    /* second run */
    second_run(outputFileName, &ic, &labelHead, &codeHead);


    entriesFileName = getNewFileName(filename, entriesSuffix);
    externalsFileName = getNewFileName(filename, externalsSuffix);
    objectFileName = getNewFileName(filename, objectSuffix);

    entries = fopen(entriesFileName, "w");
    externals = fopen(externalsFileName, "w");
    object = fopen(objectFileName, "w");

    print_code_word_list_binary(object,&codeHead);
    print_data_word_list_binary(object,&dataImgHead);

    free_label_list(&labelHead);
    free_code_word_list(&codeHead);
    free_data_word_list(&dataImgHead);
}