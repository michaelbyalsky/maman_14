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

void process_line(char *line, long *ic, long *dc, DataWord **dataImgHead, Label **labelHead, CodeWord **codeHead);

int find_label(char *line, char *label, unsigned long *line_index);

int
handle_directive(char *line, unsigned long *line_index, long *ic, long *dc, DataWord **dataImgHead, Label **labelHead, char *label);

int
handle_string_directive(const char *line, unsigned long *line_index, long *ic, long *dc, DataWord **dataImgHead, Label **labelHead,
                        char *label);

void handle_data_directive(const char *line, unsigned long *line_index, long *ic, long *dc, DataWord **dataImgHead, Label **labelHead,
                           char *label);

void handle_extern_directive(const char *line, unsigned long *line_index, long *ic, long *dc, DataWord **dataImgHead,
                             Label **labelHead, char *label);

void handle_entry_directive(const char *line, unsigned long *line_index, long *ic, long *dc, DataWord **dataImgHead,
                            Label **labelHead, char *label);

int handle_instruction(char *line, unsigned long *line_index, long *ic, long *dc, Label **labelHead, CodeWord **codeHead,
                       char *label);

int first_run(char *filename, long *ic, long *dc, DataWord **dataImgHead, Label **labelHead, CodeWord **codeHead) {
    /* read the file */
    char line[MAX_LINE_LENGTH];
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: could not open file %s\n", filename);
        return -1;
    }

    line_number = 1;
    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        process_line(line, ic, dc, dataImgHead, labelHead, codeHead);
        line_number++;
    }
    return 1;
}

void process_line(char *line, long *ic, long *dc, DataWord **dataImgHead, Label **labelHead, CodeWord **codeHead) {
    /*line index*/
    unsigned long int i = 0;
    char label[MAX_LINE_LENGTH];
    if (!is_line_80_chars_long(line)) {
        logger_error("line is more than 80 chars", line_number);
        return;
    }

    SKIP_WHITE_SPACES(line, i);

    label[0] = '\0';
    /* check if line contains a label */
    is_label = find_label(line, label, &i);
    if (is_label) {
        /* check if the label is valid */
        FuncResult label_valid = is_valid_label(label);
        if (!label_valid.result) {
            logger_error((const char *) label_valid.message, line_number);
            return;
        }
    }


    SKIP_WHITE_SPACES(line, i);
    if (is_directive(line, &i)) {
        i++;
        handle_directive(line, &i, ic, dc, dataImgHead, labelHead, label);
        return;
    }

    if (is_instruction(&line[i])) {
        handle_instruction(line, &i, ic, dc, labelHead, codeHead, label);
        return;
    }

    if (is_empty_line(line)) {
        logger_warning("empty line", line_number);
        return;
    }
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
            logger_error((const char *) result.message, line_number);
        }
        operand_1_address_method = result.result;
    } else if (instruction.numberOfOperands == 2) {
        FuncResult result = get_operand_from_string(operand_1_string, instruction, &operand_1, 1);
        if (!result.result) {
            logger_error((const char *) result.message, line_number);
        }
        operand_1_address_method = result.result;
    }

    if (num_of_operands == 2) {
        FuncResult result = get_operand_from_string(operand_2_string, instruction, &operand_2, 0);
        if (!result.result) {
            logger_error((const char *) result.message, line_number);
        }
        operand_2_address_method = result.result;
    }

    if (is_label) {
        if (labelExists(labelHead, label)) {
            logger_error("label already exists", line_number);
            is_error = 1;
            return 0;
        }
        insertLabelNode(labelHead, label, line_address, CODE_LABEL);
    }


    if (num_of_operands == 0) {
        l += 1;
        insertInstructionCodeWord(codeHead, 0, instruction.opcode, 0, l, *ic, line_address);
        line_address++;
    } else if (num_of_operands == 1) {
        if (operand_1_address_method == REGISTER_DIRECT) {
            l += 2;
            insertInstructionCodeWord(codeHead, 0, instruction.opcode, operand_1_address_method, l, *ic, line_address);
            line_address++;
            insertRegisterCodeWord(codeHead, 0, operand_1.NameLabelUnion.register_operand.registerNumber, line_address);
            line_address++;
        } else if (operand_1_address_method == DIRECT) {
            l += 2;
            insertInstructionCodeWord(codeHead, 0, instruction.opcode, operand_1_address_method, l, *ic, line_address);
            line_address++;
            insertDataLabelCodeWord(codeHead, operand_1.NameLabelUnion.label, ONE_ZERO, line_address);
            line_address++;
        } else if (operand_1_address_method == IMMEDIATE) {
            l += 2;
            insertInstructionCodeWord(codeHead, 0, instruction.opcode, operand_1_address_method, l, *ic, line_address);
            line_address++;
            insertDataNumberCodeWord(codeHead, operand_1.NameLabelUnion.number, ZERO, line_address);
            line_address++;
        }
    } else {
        if (operand_1_address_method == REGISTER_DIRECT && operand_2_address_method == REGISTER_DIRECT) {
            l += 2;
            insertInstructionCodeWord(codeHead, operand_1_address_method, instruction.opcode, operand_2_address_method, l, *ic,
                                      line_address);
            line_address++;
            insertRegisterCodeWord(codeHead, operand_1.NameLabelUnion.register_operand.registerNumber,
                                   operand_2.NameLabelUnion.register_operand.registerNumber, line_address);
            line_address++;
        } else if (operand_1_address_method == REGISTER_DIRECT && operand_2_address_method == DIRECT) {
            l += 3;
            insertInstructionCodeWord(codeHead, operand_1_address_method, instruction.opcode, operand_2_address_method, l, *ic,
                                      line_address);
            line_address++;
            insertRegisterCodeWord(codeHead, operand_1.NameLabelUnion.register_operand.registerNumber, 0, line_address);
            line_address++;
            insertDataLabelCodeWord(codeHead, operand_2.NameLabelUnion.label, ONE_ZERO, line_address);
            line_address++;
        } else if (operand_1_address_method == REGISTER_DIRECT && operand_2_address_method == IMMEDIATE) {
            l += 3;
            insertInstructionCodeWord(codeHead, operand_1_address_method, instruction.opcode, operand_2_address_method, l, *ic,
                                      line_address);
            line_address++;
            insertRegisterCodeWord(codeHead, operand_1.NameLabelUnion.register_operand.registerNumber, 0, line_address);
            line_address++;
            insertDataNumberCodeWord(codeHead, operand_2.NameLabelUnion.number, ZERO, line_address);
            line_address++;
        } else if (operand_1_address_method == DIRECT && operand_2_address_method == REGISTER_DIRECT) {
            l += 3;
            insertInstructionCodeWord(codeHead, operand_1_address_method, instruction.opcode, operand_2_address_method, l, *ic,
                                      line_address);
            line_address++;
            insertRegisterCodeWord(codeHead, 0, operand_2.NameLabelUnion.register_operand.registerNumber, line_address);
            line_address++;
            insertDataLabelCodeWord(codeHead, operand_1.NameLabelUnion.label, ONE_ZERO, line_address);
            line_address++;
        } else if (operand_1_address_method == DIRECT && operand_2_address_method == DIRECT) {
            l += 3;
            insertInstructionCodeWord(codeHead, operand_1_address_method, instruction.opcode, operand_2_address_method, l, *ic,
                                      line_address);
            line_address++;
            insertDataLabelCodeWord(codeHead, operand_1.NameLabelUnion.label, ONE_ZERO, line_address);
            line_address++;
            insertDataLabelCodeWord(codeHead, operand_2.NameLabelUnion.label, ONE_ZERO, line_address);
            line_address++;
        } else if (operand_1_address_method == DIRECT && operand_2_address_method == IMMEDIATE) {
            l += 3;
            insertInstructionCodeWord(codeHead, operand_1_address_method, instruction.opcode, operand_2_address_method, l, *ic,
                                      line_address);
            line_address++;
            insertDataLabelCodeWord(codeHead, operand_1.NameLabelUnion.label, ONE_ZERO, line_address);
            line_address++;
            insertDataNumberCodeWord(codeHead, operand_2.NameLabelUnion.number, ZERO, line_address);
            line_address++;
        } else if (operand_1_address_method == IMMEDIATE && operand_2_address_method == REGISTER_DIRECT) {
            l += 3;
            insertInstructionCodeWord(codeHead, operand_1_address_method, instruction.opcode, operand_2_address_method, l, *ic,
                                      line_address);
            line_address++;
            insertRegisterCodeWord(codeHead, 0, operand_2.NameLabelUnion.register_operand.registerNumber, line_address);
            line_address++;
            insertDataNumberCodeWord(codeHead, operand_1.NameLabelUnion.number, ZERO, line_address);
            line_address++;
        } else if (operand_1_address_method == IMMEDIATE && operand_2_address_method == DIRECT) {
            l += 3;
            insertInstructionCodeWord(codeHead, operand_1_address_method, instruction.opcode, operand_2_address_method, l, *ic,
                                      line_address);
            line_address++;
            insertDataNumberCodeWord(codeHead, operand_1.NameLabelUnion.number, ZERO, line_address);
            line_address++;
            insertDataLabelCodeWord(codeHead, operand_2.NameLabelUnion.label, ONE_ZERO, line_address);
            line_address++;
        } else if (operand_1_address_method == IMMEDIATE && operand_2_address_method == IMMEDIATE) {
            l += 3;
            insertInstructionCodeWord(codeHead, operand_1_address_method, instruction.opcode, operand_2_address_method, l, *ic,
                                      line_address);
            line_address++;
            insertDataNumberCodeWord(codeHead, operand_1.NameLabelUnion.number, ZERO, line_address);
            line_address++;
            insertDataNumberCodeWord(codeHead, operand_2.NameLabelUnion.number, ZERO, line_address);
            line_address++;
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


int
handle_directive(char *line, unsigned long *line_index, long *ic, long *dc, DataWord **dataImgHead, Label **labelHead, char *label) {
    /* extract directive name */
    int directive = find_directive_by_name(&line[*line_index]);
    if (directive == DIRECTIVE_NOT_FOUND) {
        logger_error("directive not found", line_number);
        return 0;
    }

    if (directive == DATA) {
        handle_data_directive(line, line_index, ic, dc, dataImgHead, labelHead, label);
        return 1;
    } else if (directive == STRING) {
        handle_string_directive(line, line_index, ic, dc, dataImgHead, labelHead, label);
        return 1;
    } else if (directive == ENTRY) {
        handle_entry_directive(line, line_index, ic, dc, dataImgHead, labelHead, label);
        if (is_label) {
            logger_warning("Warning: entry directive entry cannot be followed by a label", line_number);
        }
        return 1;
    } else if (directive == EXTERN) {
        /* move the line_index 6 characters forward to skip the "extern" */
        handle_extern_directive(line, line_index, ic, dc, dataImgHead, labelHead, label);
        return 1;
    }
    return 0;
}

void handle_entry_directive(const char *line, unsigned long *line_index, long *ic, long *dc, DataWord **dataImgHead,
                            Label **labelHead, char *label) {
    /**line_index+=strlen("entry");
    char entry_label[MAX_LABEL_SIZE];
    entry_label[0] = '\0';
    SKIP_WHITE_SPACES(line, *line_index);
    get_label_from_string(&line[*line_index], entry_label);
    if (labelExists(labelHead, label)) {
        logger_error("label already exists", line_number);
        is_error = 1;
        return;
    }
    insertLabelNode(labelHead, entry_label, -1, ENTRY_LABEL);*/
}

int
handle_string_directive(const char *line, unsigned long *line_index, long *ic, long *dc, DataWord **dataImgHead, Label **labelHead,
                        char *label) {
    int temp_index;
    /* move the line_index 6 characters forward to skip the "string" */
    *line_index += 6;
    SKIP_WHITE_SPACES(line, *line_index);
    /* check if the next character is a double quote */
    if (line[*line_index] != '"') {
        logger_error("string directive must be followed by a string", line_number);
        return 0;
    }
    (*line_index)++;
    temp_index = *line_index;

    /* check there is closing double quote */
    while (line[temp_index] != '"') {
        if (line[temp_index] == '\0') {
            logger_error("string directive must be followed by a string", line_number);
            return 0;
        }
        temp_index++;
    }
    if (is_label) {
        if (labelExists(labelHead, label)) {
            logger_error("label already exists", line_number);
            is_error = 1;
            return 0;
        }
        insertLabelNode(labelHead, label, line_address, DATA_LABEL);
    }

    /* go until the next double quote and add each character to the data image */
    while (1) {
        char temp_string[2] = {'\0'};
        temp_string[0] = line[*line_index];
        temp_string[1] = '\0';
        if (line[*line_index] == '"') {
            temp_string[0] = '\0';
            insertStringDataWord(dataImgHead, temp_string, line_address);
            (*dc)++;
            line_address++;
            (*line_index)++;
            break;
        } else {
            insertStringDataWord(dataImgHead, temp_string, line_address);
        }
        (*dc)++;
        line_address++;
        (*line_index)++;
    }
    (*line_index)++;
    return 1;
}

void handle_extern_directive(const char *line, unsigned long *line_index, long *ic, long *dc, DataWord **dataImgHead,
                             Label **labelHead, char *label) {
    char extern_label[MAX_LABEL_SIZE];
    *line_index += strlen("extern");
    extern_label[0] = '\0';
    SKIP_WHITE_SPACES(line, *line_index);
    get_label_from_string(&line[*line_index], extern_label);
    if (labelExists(labelHead, label)) {
        logger_error("label already exists", line_number);
        is_error = 1;
        return;
    }
    insertLabelNode(labelHead, extern_label, 0, EXTERN_LABEL);
}

void handle_data_directive(const char *line, unsigned long *line_index, long *ic, long *dc, DataWord **dataImgHead, Label **labelHead,
                           char *label) {
    char line_copy[MAX_LINE_LENGTH];
    /* move the line_index 5 characters forward to skip the "data" */
    int start_dc = *dc;
    int temp_index = 0;
    int start_address = line_address;
    *line_index += 5;
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
        /* if - */
        if (line_copy[temp_index] == '-') {
            temp_num[0] = '-';
            temp_index++;
        }
        /* if number */
        if (isdigit(line_copy[temp_index])) {
            char *endPrt;
            int converted;
            while (isdigit(line_copy[temp_index])) {
                temp_num[strlen(temp_num)] = line_copy[temp_index];
                temp_index++;
            }
            converted = strtol(temp_num, &endPrt, 10);
            insertNumberDataWord(dataImgHead, converted, line_address);
            line_address++;
            (*dc)++;
            continue;
        }
        temp_index++;
    }

    if (*dc - start_dc != 0) {
        if (is_label) {
            if (labelExists(labelHead, label)) {
                logger_error("label already exists", line_number);
                is_error = 1;
            }
            insertLabelNode(labelHead, label, start_address, DATA_LABEL);
        }
    } else {
        is_error = 1;
        logger_error("data directive must be followed by a number", line_number);
    }
}
