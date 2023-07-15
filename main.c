#include "pre_assembler.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    int i;
    char** outputFileNames = malloc(sizeof(char*) * sizeof(argc - 1));
    for (i = 1; i < argc; ++i) {
        const char* filename = argv[i];
        char* outputFileName = pre_assemble(filename);

        int outputLength = strlen(outputFileName);

        /* add this str to the array of output file names */
        outputFileNames[i - 1] = malloc(sizeof(char) * outputLength);

        strcpy(outputFileNames[i - 1], outputFileName);

    }

    for (i = 0; i < argc - 1; ++i) {
        printf("%s\n", outputFileNames[i]);
    }

    for (i = 0; i < argc - 1; ++i) {
        free(outputFileNames[i]);
    }

    free(outputFileNames);

    return 0;
}