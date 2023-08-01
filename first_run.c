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

int process_line(char *line, long *ic, long *dc, DataWord *data_img, Label **labelHead, CodeWord **codeHead);

int find_label(char *line, char *label, unsigned long *line_index);

int
handle_directive(char *line, unsigned long *line_index, long *ic, long *dc, DataWord *data_img, Label **labelHead, char *label);

int is_directive(const char *line, const unsigned long *line_index);

int
handle_string_directive(const char *line, unsigned long *line_index, long *ic, long *dc, DataWord *data_img, Label **labelHead,
                        char *label);

int handle_data_directive(const char *line, unsigned long *line_index, long *ic, long *dc, DataWord *data_img, Label **labelHead,
                          char *label);

int handle_entry_or_extern_directive(const char *line, unsigned long *line_index, long *ic, long *dc, DataWord *data_img,
                                     Label **labelHead, char *label);

int handle_instruction(char *line, unsigned long *line_index, long *ic, long *dc, Label **labelHead, CodeWord **codeHead,
                       char *label);

int first_run(char *filename, long *ic, long *dc, DataWord *data_img, Label **labelHead, CodeWord **codeHead) {
    /* read the file */
    char line[MAX_LINE_LENGTH];
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: could not open file %s\n", filename);
        return -1;
    }

    line_number = 1;
    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        process_line(line, ic, dc, data_img, labelHead, codeHead);
        line_number++;
    }
    return 1;
}

int process_line(char *line, long *ic, long *dc, DataWord *data_img, Label **labelHead, CodeWord **codeHead) {
    /*line index*/
    unsigned long int i = 0;
    char label[MAX_LINE_LENGTH];
    if (!is_line_80_chars_long(line)) {
        logger_error("line is more than 80 chars", line_number);
        return 0;
    }

    SKIP_WHITE_SPACES(line, i);

    label[0] = '\0';
    /* check if line contains a label */
    is_label = find_label(line, label, &i);
    if (is_label) {
        /* check if the label is valid */
        FuncResult label_valid = is_valid_label(label);
        if (!label_valid.result) {
            logger_error(label_valid.message, line_number);
            return 0;
        }
    }


    SKIP_WHITE_SPACES(line, i);
    if (is_directive(line, &i)) {
        i++;
        handle_directive(line, &i, ic, dc, data_img, labelHead, label);
        return 1;
    }

    if (is_instruction(&line[i])) {
        handle_instruction(line, &i, ic, dc, labelHead, codeHead, label);
        return 1;
    }

    if (is_empty_line(line)) {
        return 0;
    }

    return 1;
}

int handle_instruction(char *line, unsigned long *line_index, long *ic, long *dc, Label **labelHead, CodeWord **codeHead,
                       char *label) {
    int num_of_operands = 0;
    enum AddressMethod operand_1_address_method;
    enum AddressMethod operand_2_address_method;
    char operand_1_string[MAX_LINE_LENGTH];
    char operand_2_string[MAX_LINE_LENGTH];
    char line_copy[MAX_LINE_LENGTH];
    char *token;
    Operand operand_1;
    Operand operand_2;
    int l = 0;

    Instruction instruction = findInstructionByName(&line[*line_index]);
    if (instruction.opcode == -1) {
        logger_error("instruction not found", line_number);
        return 0;
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
    token = strtok(NULL, ",");
    if (token != NULL && *token != 13 && *token != 10) {
        strcpy(operand_2_string, token);
        operand_2_string[strlen(operand_2_string)] = '\0';
        num_of_operands++;
    }

    if (num_of_operands != instruction.numberOfOperands) {
        printf("Error: instruction %s must have %d operands in Line: %d\n", instruction.name, instruction.numberOfOperands, line_number);
        return -1;
    }

    if (instruction.numberOfOperands == 1) {
        FuncResult result = get_operand_from_string(operand_1_string, instruction, &operand_1, 0);
        if (!result.result) {
            logger_error(result.message, line_number);
        }
        operand_1_address_method = result.result;
    } else if (instruction.numberOfOperands == 2) {
        FuncResult result = get_operand_from_string(operand_1_string, instruction, &operand_1, 1);
        if (!result.result) {
            logger_error(result.message, line_number);
        }
        operand_1_address_method = result.result;
    }

    if (num_of_operands == 2) {
        FuncResult result = get_operand_from_string(operand_2_string, instruction, &operand_2, 0);
        if (!result.result) {
            logger_error(result.message, line_number);
        }
        operand_2_address_method = result.result;
    }

    if (is_label) {
        insertLabelNode(labelHead, label, *ic, CODE_LABEL);
    }


    if (num_of_operands == 0) {
        insertInstructionCodeWord(codeHead, 0, instruction.opcode, 0);
        l += 1;
    } else if (num_of_operands == 1) {
        if (operand_1_address_method == REGISTER_DIRECT) {
            insertInstructionCodeWord(codeHead, 0, instruction.opcode, operand_1_address_method);
            insertRegisterCodeWord(codeHead, 0, operand_1.NameLabelUnion.register_operand.registerNumber);
            l += 2;
        } else if (operand_1_address_method == DIRECT) {
            insertInstructionCodeWord(codeHead, 0, instruction.opcode, operand_1_address_method);
            insertDataLabelCodeWord(codeHead, operand_1.NameLabelUnion.label, ONE_ZERO);
            l += 2;
        } else if (operand_1_address_method == IMMEDIATE) {
            insertInstructionCodeWord(codeHead, 0, instruction.opcode, operand_1_address_method);
            insertDataNumberCodeWord(codeHead, operand_1.NameLabelUnion.number, ZERO);
            l += 2;
        }
    } else {
        if (operand_1_address_method == REGISTER_DIRECT && operand_2_address_method == REGISTER_DIRECT) {
            insertInstructionCodeWord(codeHead, operand_1_address_method, instruction.opcode, operand_2_address_method);
            insertRegisterCodeWord(codeHead, operand_1.NameLabelUnion.register_operand.registerNumber,
                                   operand_2.NameLabelUnion.register_operand.registerNumber);
            l += 2;
        } else if (operand_1_address_method == REGISTER_DIRECT && operand_2_address_method == DIRECT) {
            insertInstructionCodeWord(codeHead, operand_1_address_method, instruction.opcode, operand_2_address_method);
            insertRegisterCodeWord(codeHead, operand_1.NameLabelUnion.register_operand.registerNumber, 0);
            insertDataLabelCodeWord(codeHead, operand_2.NameLabelUnion.label, ONE_ZERO);
            l += 3;
        } else if (operand_1_address_method == REGISTER_DIRECT && operand_2_address_method == IMMEDIATE) {
            insertInstructionCodeWord(codeHead, operand_1_address_method, instruction.opcode, operand_2_address_method);
            insertRegisterCodeWord(codeHead, operand_1.NameLabelUnion.register_operand.registerNumber, 0);
            insertDataNumberCodeWord(codeHead, operand_2.NameLabelUnion.number, ZERO);
            l += 3;
        } else if (operand_1_address_method == DIRECT && operand_2_address_method == REGISTER_DIRECT) {
            insertInstructionCodeWord(codeHead, operand_1_address_method, instruction.opcode, operand_2_address_method);
            insertRegisterCodeWord(codeHead, 0, operand_2.NameLabelUnion.register_operand.registerNumber);
            insertDataLabelCodeWord(codeHead, operand_1.NameLabelUnion.label, ONE_ZERO);
            l += 3;
        } else if (operand_1_address_method == DIRECT && operand_2_address_method == DIRECT) {
            insertInstructionCodeWord(codeHead, operand_1_address_method, instruction.opcode, operand_2_address_method);
            insertDataLabelCodeWord(codeHead, operand_1.NameLabelUnion.label, ONE_ZERO);
            insertDataLabelCodeWord(codeHead, operand_2.NameLabelUnion.label, ONE_ZERO);
            l += 3;
        } else if (operand_1_address_method == DIRECT && operand_2_address_method == IMMEDIATE) {
            insertInstructionCodeWord(codeHead, operand_1_address_method, instruction.opcode, operand_2_address_method);
            insertDataLabelCodeWord(codeHead, operand_1.NameLabelUnion.label, ONE_ZERO);
            insertDataNumberCodeWord(codeHead, operand_2.NameLabelUnion.number, ZERO);
            l += 3;
        } else if (operand_1_address_method == IMMEDIATE && operand_2_address_method == REGISTER_DIRECT) {
            insertInstructionCodeWord(codeHead, operand_1_address_method, instruction.opcode, operand_2_address_method);
            insertRegisterCodeWord(codeHead, 0, operand_2.NameLabelUnion.register_operand.registerNumber);
            insertDataNumberCodeWord(codeHead, operand_1.NameLabelUnion.number, ZERO);
            l += 3;
        } else if (operand_1_address_method == IMMEDIATE && operand_2_address_method == DIRECT) {
            insertInstructionCodeWord(codeHead, operand_1_address_method, instruction.opcode, operand_2_address_method);
            insertDataNumberCodeWord(codeHead, operand_1.NameLabelUnion.number, ZERO);
            insertDataLabelCodeWord(codeHead, operand_2.NameLabelUnion.label, ONE_ZERO);
            l += 3;
        } else if (operand_1_address_method == IMMEDIATE && operand_2_address_method == IMMEDIATE) {
            insertInstructionCodeWord(codeHead, operand_1_address_method, instruction.opcode, operand_2_address_method);
            insertDataNumberCodeWord(codeHead, operand_1.NameLabelUnion.number, ZERO);
            insertDataNumberCodeWord(codeHead, operand_2.NameLabelUnion.number, ZERO);
            l += 3;
        }

    }
    (*ic) += l;
    return 1;
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


int is_directive(const char *line, const unsigned long *line_index) {
    if (line[*line_index] == '.') {
        return 1;
    }
    return 0;
}

int
handle_directive(char *line, unsigned long *line_index, long *ic, long *dc, DataWord *data_img, Label **labelHead, char *label) {
    /* extract directive name */
    int directive = find_directive_by_name(&line[*line_index]);
    if (directive == DIRECTIVE_NOT_FOUND) {
        logger_error("Error: directive not found", line_number);
        return 0;
    }

    if (directive == DATA) {
        handle_data_directive(line, line_index, ic, dc, data_img, labelHead, label);
    } else if (directive == STRING) {
        handle_string_directive(line, line_index, ic, dc, data_img, labelHead, label);
    } else if (directive == ENTRY) {
        /* move the line_index 5 characters forward to skip the "entry" */
        *line_index += 5;
        handle_entry_or_extern_directive(line, line_index, ic, dc, data_img, labelHead, label);
    } else if (directive == EXTERN) {
        /* move the line_index 6 characters forward to skip the "extern" */
        *line_index += 6;
        handle_entry_or_extern_directive(line, line_index, ic, dc, data_img, labelHead, label);
    }
}

int
handle_string_directive(const char *line, unsigned long *line_index, long *ic, long *dc, DataWord *data_img, Label **labelHead,
                        char *label) {
    int temp_index;
    /* move the line_index 6 characters forward to skip the "string" */
    *line_index += 6;
    SKIP_WHITE_SPACES(line, *line_index);
    /* check if the next character is a double quote */
    if (line[*line_index] != '"') {
        logger_error("Error: string directive must be followed by a string", line_number);
        return 0;
    }
    (*line_index)++;
    temp_index = *line_index;

    /* check there is closing double quote */
    while (line[temp_index] != '"') {
        if (line[temp_index] == '\0') {
            logger_error("Error: string directive must be followed by a string", line_number);
            return 0;
        }
        temp_index++;
    }

    insertLabelNode(labelHead, label, *dc, DATA_LABEL);

    /* go until the next double quote and add each character to the data image */
    while (line[*line_index] != '"') {
        data_img[*dc].datatype = STRING;
        data_img[*dc].NumberStringUnion.string[0] = line[*line_index];
        data_img[*dc].NumberStringUnion.string[1] = '\0';
        (*dc)++;
        (*line_index)++;
    }
    (*line_index)++;
}

int handle_entry_or_extern_directive(const char *line, unsigned long *line_index, long *ic, long *dc, DataWord *data_img,
                                     Label **labelHead, char *label) {
    char extern_label[MAX_LABEL_SIZE];
    extern_label[0] = '\0';
    SKIP_WHITE_SPACES(line, *line_index);
    get_label_from_string(&line[*line_index], extern_label);
    insertLabelNode(labelHead, extern_label, 0, EXTERN_LABEL);
}

int handle_data_directive(const char *line, unsigned long *line_index, long *ic, long *dc, DataWord *data_img, Label **labelHead,
                          char *label) {
    /* move the line_index 4 characters forward to skip the "data" */
    *line_index += 5;
    char line_copy[MAX_LINE_LENGTH];
    line_copy[0] = '\0';
    strcpy(line_copy, &line[*line_index]);
    remove_white_spaces(line_copy, 0);
    int temp_index = 0;
    int start_dc = *dc;
    /* while is a character */
    while (line_copy[temp_index] != '\000' && line_copy[temp_index] != "\0" && line_copy[temp_index] != "\n") {
        char temp_num[MAX_LINE_LENGTH] = {'\0'};
        if (line_copy[temp_index] == ',') {
            temp_index++;
            continue;
        }
        /* if - */
        if (line_copy[temp_index] == '-') {
            temp_num[0] = '-';
            temp_index++;
        }
        /* if number */
        if (isdigit(line_copy[temp_index])) {
            while (isdigit(line_copy[temp_index])) {
                temp_num[strlen(temp_num)] = line_copy[temp_index];
                temp_index++;
            }
            data_img[*dc].datatype = DATA;
            data_img[*dc].NumberStringUnion.number = atoi(temp_num);
            (*dc)++;
            continue;
        }
        temp_index++;
    }
    if (*dc - start_dc != 0) {
        insertLabelNode(labelHead, label, start_dc, DATA_LABEL);
    }
}
