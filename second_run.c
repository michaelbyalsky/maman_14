#include "second_run.h"
#include "tables.h"
#include "code.h"

int line_number_2;

void process_line_second_run(char *line, long *ic, long *dc, DataWord **dataImgHead, Label **labelHead, CodeWord **codeHead);

void skip_label(char *line, unsigned long *line_index);

int second_run(char *filename, long *ic, long *dc, DataWord **dataImgHead, Label **labelHead, CodeWord **codeHead) {
    char line[MAX_LINE_LENGTH];
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: could not open file %s\n", filename);
        return -1;
    }

    line_number_2 = 1;
    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        process_line_second_run(line, ic, dc, dataImgHead, labelHead, codeHead);
        line_number_2++;
    }
    return 1;
}

void process_line_second_run(char *line, long *ic, long *dc, DataWord **dataImgHead, Label **labelHead, CodeWord **codeHead) {
    unsigned long int i = 0;
    SKIP_WHITE_SPACES(line, i);
    skip_label(line, &i);
    SKIP_WHITE_SPACES(line, i);

    if (is_directive(line, &i)) {
        int directive;
        i++;
        directive = find_directive_by_name(&line[i]);
        if (directive == DIRECTIVE_NOT_FOUND) {
            logger_error("Invalid directive", line_number_2);
        }
        if (directive == DATA || directive == STRING || directive == EXTERN) {
            return;
        } else if (directive == ENTRY) {
            FuncResult is_valid_label_result;
            char label[MAX_LINE_LENGTH];
            i += strlen("entry");
            SKIP_WHITE_SPACES(line, i);
            get_label_from_string(&line[i], label);
            is_valid_label_result = is_valid_label(label);
            if (!is_valid_label_result.result) {
                is_error = 1;
                logger_error((const char *) is_valid_label_result.message, line_number_2);
            }
            /* test it */
            if (!updateLabelType(labelHead, label, (enum LabelType) ENTRY)) {
                is_error = 1;
                logger_error("update label operation failed", line_number_2);
            };
            return;
        }
    }

    if (is_instruction(&line[i])) {
        int l;
        int j;
        CodeWord *codeWord = findCodeWordByIC(codeHead, *ic);
        if (codeWord == NULL) {
            logger_error("Code word not found", line_number_2);
        }
        l = codeWord->CodeWordUnion.instruction.totalWords;
        for (j = 1; j < l; ++j) {
            codeWord = codeWord->next;
            if (codeWord == NULL) {
                logger_error("Code word not found", line_number_2);
                return;
            }
            if (codeWord->codeWordType == DATA_LABEL_WORD) {
                Label *label = findLabelByName(labelHead, codeWord->CodeWordUnion.data.label);
                if (label == NULL) {
                    logger_error("Label not found", line_number_2);
                    return;
                }
                codeWord->CodeWordUnion.data.value = label->address;
                codeWord->codeWordType = DATA_NUMBER_WORD;
            }
        }
        (*ic) += l;
    }
}

void skip_label(char *line, unsigned long *line_index) {
    char label[MAX_LABEL_SIZE];
    char *colon = strchr(line, ':');
    if (colon == NULL) {
        return;
    }

    sscanf(line, "%[^:]", label); /* read until ":" the [^:] means read until ":" */
    label[strlen(label)] = '\0'; /* add the null terminator to the label */
    /* move the line pointer to the next character after the label */
    remove_white_spaces(label, 0);
    *line_index += strlen(label) + 1;
}