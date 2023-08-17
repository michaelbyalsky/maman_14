#include "second_run.h"
#include "tables.h"
#include "code.h"

int line_number_2;

void process_line_second_run(char *line, unsigned int *ic, Label **labelHead, CodeWord **codeHead);

/**
 * @brief the function skips the label in the line
 * @param line - the line to process
 * @param line_index - the index of the line
 */
void skip_label(char *line, unsigned long *line_index);

void second_run(char *filename, unsigned int *ic, Label **labelHead, CodeWord **codeHead) {
    char line[MAX_LINE_LENGTH];
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: could not open file %s\n", filename);
        return;
    }

    line_number_2 = 1;
    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        process_line_second_run(line, ic, labelHead, codeHead);
        line_number_2++;
    }
    fclose(file);
}

/**
 * @brief the function processes single line in the second run
 * @param line
 * @param ic
 * @param labelHead
 * @param codeHead
 */
void process_line_second_run(char *line, unsigned int *ic, Label **labelHead,
                             CodeWord **codeHead) {
    unsigned long int i = 0;
    SKIP_WHITE_SPACES(line, i);
    skip_label(line, &i);
    SKIP_WHITE_SPACES(line, i);

    /* if is directive we should update the label if it is entry */
    if (is_directive(line, &i)) {
        Directive directive;
        i++;
        directive = find_directive_by_name(&line[i]);
        if (directive.directive == DIRECTIVE_NOT_FOUND) {
            logger_error("Invalid directive", line_number_2);
            is_error = 1;
        }
        /* skip the directive data, string and extern */
        if (directive.directive == DATA || directive.directive == STRING || directive.directive == EXTERN) {
            return;
        } else if (directive.directive == ENTRY) {
            FuncResult is_valid_label_result;
            char label[MAX_LINE_LENGTH];
            i += strlen(directive.name);
            SKIP_WHITE_SPACES(line, i);
            get_label_from_string(&line[i], label);
            is_valid_label_result = is_valid_label(label);
            if (!is_valid_label_result.result) {
                logger_error((const char *) is_valid_label_result.message, line_number_2);
                is_error = 1;
                return;
            }
            /* update the label type to entry */
            if (!update_label_type(labelHead, label, (enum LabelType) ENTRY)) {
                logger_error("update label operation failed", line_number_2);
                is_error = 1;
                return;
            };
            return;
        }
    }

    /* if is instruction we need to update the code of all the label to their address */
    if (is_instruction(&line[i])) {
        int l;
        int j;
        CodeWord *codeWord = find_code_word_by_ic(codeHead, *ic);
        if (codeWord == NULL) {
            logger_error("Code word not found", line_number_2);
            is_error = 1;
            return;
        }
        l = codeWord->CodeWordUnion.instruction.totalWords;
        /* loop over the extra words, find the label and update the address */
        for (j = 1; j < l; ++j) {
            codeWord = codeWord->next;
            if (codeWord == NULL) {
                logger_error("Code word not found", line_number_2);
                is_error = 1;
                return;
            }
            if (codeWord->codeWordType == DATA_LABEL_WORD) {
                Label *label = find_label_by_name(labelHead, codeWord->CodeWordUnion.data.label);
                if (label == NULL) {
                    logger_error("Label not found", line_number_2);
                    is_error = 1;
                    return;
                }
                /* if extern are 01 */
                if (label->type == EXTERN_LABEL) {
                    /* update the are to 01 */
                    codeWord->are = ZERO_ONE;
                    /* if entry are 10 */
                    /* add the label address to the code word */
                } else if (label->type == ENTRY_LABEL || label->type == CODE_LABEL || label->type == DATA_LABEL) {
                    free(codeWord->CodeWordUnion.data.label);
                    codeWord->CodeWordUnion.data.labelAddress = label->address;
                    codeWord->codeWordType = DATA_ADDRESS_WORD;
                }
            }
        }
        /* increase the ic by the number of words */
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