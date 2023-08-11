#include "pre_assembler.h"
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>

#include "first_run.h"
#include "code.h"
#include "tables.h"
#include "second_run.h"

FILE *outputFile;
const char *directoryName = "outputs";
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
    long ic = IC_START;
    long dc = DC_START;\
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

    if (is_error) {
        printf("Error in second pass, file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    if (!is_directory_exists(directoryName)) {
        if (mkdir(directoryName, 0777) != 0) {
            printf("Error creating directory.\n");
            is_error = EXIT_FAILURE;
        }
    }

    sprintf(filePath, "%s/ps.ob", directoryName);
    outputFile = fopen(filePath, "w");

    if (outputFile == NULL) {
        printf("Error opening file.\n");
        is_error = EXIT_FAILURE;
    }

    print_code_word_list_binary(outputFile,&codeHead);
    print_data_word_list_binary(outputFile,&dataImgHead);

    free_label_list(&labelHead);
    free_code_word_list(&codeHead);
    free_data_word_list(&dataImgHead);
}