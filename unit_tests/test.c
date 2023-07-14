#include <stdio.h>
#include <string.h>

#include "../pre_assembler.h"

int compareFiles(const char* file1, const char* file2) {
    int lineNum = 1;
    int different = 0;
    char line2[100];
    char line1[100];
    FILE* fp1 = fopen(file1, "r");
    FILE* fp2 = fopen(file2, "r");

    if (fp1 == NULL || fp2 == NULL) {
        printf("Error opening files\n");
        return;
    }

    while (fgets(line1, sizeof(line1), fp1) && fgets(line2, sizeof(line2), fp2)) {
        size_t len1 = strlen(line1);
        size_t len2 = strlen(line2);
        if (line1[len1 - 1] == '\n') {
            line1[len1 - 1] = '\0';
        }
        if (line2[len2 - 1] == '\n') {
            line2[len2 - 1] = '\0';
        }

        if (strcmp(line1, line2) != 0) {
            printf("Difference at line %d\n", lineNum);
            different = 1;
            break;
        }

        lineNum++;
    }

    fclose(fp1);
    fclose(fp2);

    if (!different && feof(fp1) && feof(fp2)) {
        printf("Files match!\n");
        return 1;
    } else {
        printf("Files differ!\n");
        return 0;
    }

}

int main(int argc, char* argv[]) {
    int result;
    int test_result;
    pre_assemble("./initial.as", "output_after_pre.as");
    result = compareFiles("output_after_pre.as", "./expected_after_pre.as");
    if (result) {
        printf("Test passed!\n");
        test_result = 1;
    } else {
        printf("Test failed!\n");
        test_result = 0;
    }
    return test_result;
}


