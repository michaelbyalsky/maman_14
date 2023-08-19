#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "globals.h"
#include "helpers.h"
#include "tables.h"
#include "code.h"

int line_number;
int is_label;

void process_line(char *line, unsigned int *ic, unsigned int *dc, DataWord **dataImgHead, Label **labelHead,
                  CodeWord **codeHead);

/**
 * @brief find label in line and assign it to label
 * @param line - the line to search in
 * @param label - the label to return
 * @param line_index - the index of the line
 * @return - 1 if label found, 0 otherwise
 */
int find_label(char *line, char *label, unsigned long *line_index);

/**
 * @brief handle directive line
 * @param line - the line to handle
 * @param line_index - the index of the line
 * @param ic - instruction counter
 * @param dc - data counter
 * @param dataImgHead - data image head
 * @param labelHead - label list
 * @param label - the label of the line
 */
void
handle_directive(char *line, unsigned long *line_index, unsigned int *dc, DataWord **dataImgHead, Label **labelHead,
                 char *label);

/**
 * @brief handle string directive
 * @param line - the line to handle
 * @param line_index - the index of the line
 * @param ic - instruction counter
 * @param dc - data counter
 * @param dataImgHead - data image head
 * @param labelHead - label list
 * @param label - the label of the line
 */
void handle_string_directive(const char *line, unsigned long *line_index, unsigned int *dc, DataWord **dataImgHead,
                             Label **labelHead,
                             char *label);

/**
 * @brief handle data directive
 * @param line - the line to handle
 * @param line_index - the index of the line
 * @param dc - data counter
 * @param dataImgHead - data image head
 * @param labelHead - label list
 * @param label - the label of the line
 */
void
handle_data_directive(const char *line, const unsigned long *line_index, unsigned int *dc, DataWord **dataImgHead,
                      Label **labelHead,
                      char *label);

/**
 * @brief handle extern directive
 * @param line - the line to handle
 * @param line_index - the index of the line
 * @param dc - data counter
 * @param dataImgHead - data image list
 * @param labelHead - label list
 * @param label - the label of the line
 */
void handle_extern_directive(const char *line, unsigned long *line_index,
                             Label **labelHead, char *label);

void
handle_instruction(char *line, unsigned long *line_index, unsigned int *ic, Label **labelHead, CodeWord **codeHead,
                   char *label);

void first_run(char *filename, unsigned int *ic, unsigned int *dc, DataWord **dataImgHead, Label **labelHead,
               CodeWord **codeHead) {
    /* read the file */
    char line[MAX_LINE_LENGTH];
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: could not open file %s\n", filename);
        is_error = 1;
        return;
    }

    line_number = 1;
    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        /* check if line is longer than 80 characters */
        if (!is_line_80_chars_long(line)) {
            logger_error("line is longer than 80 characters", line_number);
            is_error = 1;
            /* read the rest of the line */
            while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
                if (is_line_80_chars_long(line)) {
                    line_number++;
                    break;
                }
            }
            continue;
        }
        process_line(line, ic, dc, dataImgHead, labelHead, codeHead);
        line_number++;
    }
    fclose(file);
}

void process_line(char *line, unsigned int *ic, unsigned int *dc, DataWord **dataImgHead, Label **labelHead,
                  CodeWord **codeHead) {
    /*line index*/
    unsigned long int i = 0;
    char label[MAX_LINE_LENGTH];

    SKIP_WHITE_SPACES(line, i);

    label[0] = '\0';
    /* check if line contains a label */
    is_label = find_label(line, label, &i);
    if (is_label) {
        /* check if the label is valid */
        FuncResult label_valid = is_valid_label(label);
        if (!label_valid.result) {
            logger_error((const char *) label_valid.message, line_number);
            is_error = 1;
            return;
        }
    }


    SKIP_WHITE_SPACES(line, i);
    /* check if the line is directive */
    if (is_directive(line, &i)) {
        /* move the line pointer to the next character after the directive . */
        i++;
        handle_directive(line, &i, dc, dataImgHead, labelHead, label);
        return;
    }

    /* check if the line is instruction */
    if (is_instruction(&line[i])) {
        handle_instruction(line, &i, ic, labelHead, codeHead, label);
        return;
    }

    /*
     * if the line is not empty this means there is not allowed assembly commands
     * since we checked the directive and instruction
     * */
    if (!is_empty_line(line)) {
        logger_error("invalid line with not allowed assembly commands", line_number);
        is_error = 1;
        return;
    }
}

void handle_instruction(char *line, unsigned long *line_index, unsigned int *ic, Label **labelHead, CodeWord **codeHead,
                        char *label) {
    int num_of_operands = 0;
    char operand_1_string[MAX_LINE_LENGTH];
    char operand_2_string[MAX_LINE_LENGTH];
    char line_copy[MAX_LINE_LENGTH];
    char *token;
    Operand operand_1;
    Operand operand_2;
    int l;
    Instruction instruction = findInstructionByName(&line[*line_index]);

    if (instruction.opcode == INSTRUCTION_NOT_FOUND) {
        logger_error("instruction not found", line_number);
        is_error = 1;
        return;
    }
    SKIP_WHITE_SPACES(line, *line_index);
    *line_index += strlen(instruction.name);
    operand_1_string[0] = '\0';
    operand_2_string[0] = '\0';
    line_copy[0] = '\0';
    strcpy(line_copy, &line[*line_index]);
    line_copy[strlen(line_copy)] = '\0';
    remove_white_spaces(line_copy, 0);
    /* get the operands string */
    token = strtok(line_copy, ",");
    if (token != NULL && *token != 13 && *token != 10) {
        strcpy(operand_1_string, token);
        operand_1_string[strlen(operand_1_string)] = '\0';
        num_of_operands++;
    }
    /* get the second operand */
    token = strtok(NULL, ",");
    if (token != NULL && *token != 13 && *token != 10) {
        strcpy(operand_2_string, token);
        operand_2_string[strlen(operand_2_string)] = '\0';
        num_of_operands++;
    }

    /* check if the number of operands is valid */
    if (num_of_operands != instruction.numberOfOperands) {
        printf("Error: instruction %s must have %d operands in Line %d\n", instruction.name,
               instruction.numberOfOperands, line_number);
        is_error = 1;
        return;
    }


    /* if number of operands is 1 there is only dest operand */
    if (instruction.numberOfOperands == 1) {
        FuncResult result = get_operand_from_string(operand_1_string, instruction, &operand_1, 0);
        if (!result.result) {
            logger_error((const char *) result.message, line_number);
            is_error = 1;
            return;
        }
        /* if number of operands is 2 there is source and dest operands */
    } else if (instruction.numberOfOperands == 2) {
        FuncResult result = get_operand_from_string(operand_1_string, instruction, &operand_1, 1);
        if (!result.result) {
            logger_error((const char *) result.message, line_number);
            is_error = 1;
            return;
        }
    }

    /* get the dest operand */
    if (num_of_operands == 2) {
        FuncResult result = get_operand_from_string(operand_2_string, instruction, &operand_2, 0);
        if (!result.result) {
            logger_error((const char *) result.message, line_number);
            is_error = 1;
            return;
        }
    }

    if (is_label) {
        if (label_exists(labelHead, label)) {
            logger_error("label already exists", line_number);
            is_error = 1;
            return;
        }
        insert_label_node(labelHead, label, line_address, CODE_LABEL);
    }


    /* insert the code words depends on the address methods */
    l = write_code_word(codeHead, instruction, ic, num_of_operands, operand_1, operand_2);
    /* update the instruction counter */
    (*ic) += l;
}

int find_label(char *line, char *label, unsigned long *line_index) {
    /* check if there is a : in the line */
    char *colon = strchr(line, ':');
    if (colon == NULL) {
        return 0;
    }

    sscanf(line, "%[^:]", label); /* read until ":" the [^:] means read until ":" */
    label[strlen(label)] = '\0'; /* add the null terminator to the label */
    /* move the line pointer to the next character after the label */
    remove_white_spaces(label, 0);
    *line_index += strlen(label) + 1;

    return 1;
}


void
handle_directive(char *line, unsigned long *line_index, unsigned int *dc, DataWord **dataImgHead, Label **labelHead,
                 char *label) {
    /* extract directive name */
    Directive directive = find_directive_by_name(&line[*line_index]);
    /* check if the directive exists */
    if (directive.directive == DIRECTIVE_NOT_FOUND) {
        logger_error("directive not found", line_number);
        is_error = 1;
        return;
    }

    /* move the line pointer to the next character after the directive */
    *line_index += strlen(directive.name);

    if (directive.directive == DATA) {
        handle_data_directive(line, line_index, dc, dataImgHead, labelHead, label);
        return;
    } else if (directive.directive == STRING) {
        handle_string_directive(line, line_index, dc, dataImgHead, labelHead, label);
        return;
    } else if (directive.directive == ENTRY) {
        if (is_label) {
            logger_warning("entry directive entry cannot be followed by a label", line_number);
        }
        return;
    } else if (directive.directive == EXTERN) {
        handle_extern_directive(line, line_index, labelHead, label);
        return;
    }
}

void handle_string_directive(const char *line, unsigned long *line_index, unsigned int *dc, DataWord **dataImgHead,
                             Label **labelHead,
                             char *label) {
    unsigned long temp_index;
    SKIP_WHITE_SPACES(line, *line_index);
    /* check if the next character is a double quote */
    if (line[*line_index] != '"') {
        logger_error("string directive must be followed by a string", line_number);
        is_error = 1;
        return;
    }
    (*line_index)++;
    temp_index = *line_index;

    /* check there is closing double quote */
    while (line[temp_index] != '"') {
        if (line[temp_index] == '\0') {
            logger_error("string directive must be followed by a string", line_number);
            is_error = 1;
            return;
        }
        temp_index++;
    }
    if (is_label) {
        if (label_exists(labelHead, label)) {
            logger_error("label already exists", line_number);
            is_error = 1;
            return;
        }
        insert_label_node(labelHead, label, line_address, DATA_LABEL);
    }

    /* go until the next double quote and add each character to the data image */
    while (1) {
        char temp_string[2] = {'\0'};
        temp_string[0] = line[*line_index];
        temp_string[1] = '\0';
        /* if reached the end of the string */
        if (line[*line_index] == '"') {
            temp_string[0] = '\0';
            insert_string_data_word(dataImgHead, temp_string, line_address);
            (*dc)++;
            line_address++;
            (*line_index)++;
            break;
        } else {
            insert_string_data_word(dataImgHead, temp_string, line_address);
        }
        (*dc)++;
        line_address++;
        (*line_index)++;
    }
    (*line_index)++;
}

void handle_extern_directive(const char *line, unsigned long *line_index,
                             Label **labelHead, char *label) {
    char extern_label[MAX_LABEL_SIZE];
    extern_label[0] = '\0';
    SKIP_WHITE_SPACES(line, *line_index);
    get_label_from_string(&line[*line_index], extern_label);
    if (label_exists(labelHead, label)) {
        logger_error("label already exists", line_number);
        is_error = 1;
        return;
    }
    remove_new_line_char_from_string(extern_label);

    insert_label_node(labelHead, extern_label, 0, EXTERN_LABEL);
}

void
handle_data_directive(const char *line, const unsigned long *line_index, unsigned int *dc, DataWord **dataImgHead,
                      Label **labelHead,
                      char *label) {
    char line_copy[MAX_LINE_LENGTH];
    unsigned int start_dc = *dc;
    int temp_index = 0;
    unsigned int start_address = line_address;
    line_copy[0] = '\0';
    strcpy(line_copy, &line[*line_index]);
    remove_white_spaces(line_copy, 0);
    /* while is a character */
    while (line_copy[temp_index] != '\000' && line_copy[temp_index] != '\0' && line_copy[temp_index] != '\n') {
        char temp_num[MAX_LINE_LENGTH] = {'\0'};
        if (line_copy[temp_index] == ',') {
            temp_index++;
            continue;
        }
        /* if negative number */
        if (line_copy[temp_index] == '-') {
            temp_num[0] = '-';
            temp_index++;
        }
        /* if number */
        if (isdigit(line_copy[temp_index])) {
            int converted;
            while (isdigit(line_copy[temp_index])) {
                temp_num[strlen(temp_num)] = line_copy[temp_index];
                temp_index++;
            }
            converted = strtol(temp_num, NULL, 10);
            /* check if the number is 12 bits signed this is the range of the data directive */
            if (is12BitsSigned(converted)) {
                insert_number_data_word(dataImgHead, converted, line_address);
                line_address++;
                (*dc)++;
                continue;
            } else {
                logger_error("number is not 12 bits signed", line_number);
                is_error = 1;
                return;
            }
        }
        temp_index++;
    }

    /* check if the data have at least one number */
    if (*dc - start_dc != 0) {
        if (is_label) {
            if (label_exists(labelHead, label)) {
                logger_error("label already exists", line_number);
                is_error = 1;
                return;
            }
            insert_label_node(labelHead, label, start_address, DATA_LABEL);
        }
    } else {
        is_error = 1;
        logger_error("data directive must be followed by a number", line_number);
        return;
    }
}
