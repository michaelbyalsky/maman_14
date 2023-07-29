#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "globals.h"
#include "helpers.h"
#include "first_run.h"

int process_line(char *line, long *ic, long *dc, DataWord *data_img, Label **labelHead, CodeWord **codeHead);

int find_label(char *line, char *label, int *line_index);

int
handle_directive(char *line, int *line_index, long *ic, long *dc, DataWord *data_img, Label **labelHead, char *label);

int is_directive(const char *line, const int *line_index);

int
handle_string_directive(const char *line, int *line_index, long *ic, long *dc, DataWord *data_img, Label **labelHead,
                        char *label);

int handle_data_directive(const char *line, int *line_index, long *ic, long *dc, DataWord *data_img, Label **labelHead,
                          char *label);

int handle_entry_or_extern_directive(const char *line, int *line_index, long *ic, long *dc, DataWord *data_img,
                                     Label **labelHead, char *label);

int handle_instruction(char *line, int *line_index, long *ic, long *dc, Label **labelHead, CodeWord **codeHead,
                       char *label);

int first_run(char *filename, long *ic, long *dc, DataWord *data_img, Label **labelHead, CodeWord **codeHead) {
    /* read the file */
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: could not open file %s\n", filename);
        return -1;
    }

    char line[MAX_LINE_LENGTH];

    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        process_line(line, ic, dc, data_img, labelHead, codeHead);
    }

}

int process_line(char *line, long *ic, long *dc, DataWord *data_img, Label **labelHead, CodeWord **codeHead) {
    /*line index*/
    int i = 0;

    SKIP_WHITE_SPACES(line, i);

    char label[MAX_LABEL_SIZE];
    label[0] = '\0';
    /* check if line contains a label */
    find_label(line, label, &i);


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
}

int handle_instruction(char *line, int *line_index, long *ic, long *dc, Label **labelHead, CodeWord **codeHead,
                       char *label) {
    Instruction instruction = findInstructionByName(&line[*line_index]);
    if (instruction.opcode == -1) {
        printf("Error: instruction not found\n");
        return -1;
    }
    SKIP_WHITE_SPACES(line, *line_index);
    /* move the line_index 4 characters forward to skip the instruction name */
    *line_index += strlen(instruction.name);
    int num_of_operands = 0;
    enum e_address operand_1_address_method;
    enum e_address operand_2_address_method;
    char operand_1_string[MAX_LINE_LENGTH];
    char operand_2_string[MAX_LINE_LENGTH];
    char line_copy[MAX_LINE_LENGTH];
    operand_1_string[0] = '\0';
    operand_2_string[0] = '\0';
    line_copy[0] = '\0';
    Operand operand_1;
    Operand operand_2;
    /* check if the instruction has operands */
    /* iterate other instruction allowedSourceAddressingMethods */
    /* create line copy from the line_index */
    strcpy(line_copy, &line[*line_index]);
    /* put \0 at the end of the line */
    line_copy[strlen(line_copy)] = '\0';
    remove_white_spaces(line_copy, 0);
    /* get the operands string */
    char *token = strtok(line_copy, ",");
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
        printf("Error: instruction %s must have %d operands\n", instruction.name, instruction.numberOfOperands);
        return -1;
    }

    if (instruction.numberOfOperands == 1) {
        operand_1_address_method = get_operand_from_string(operand_1_string, instruction, &operand_1, 0);
    } else if (instruction.numberOfOperands == 2) {
        operand_1_address_method = get_operand_from_string(operand_1_string, instruction, &operand_1, 1);
    }

    if (num_of_operands == 2) {
        operand_2_address_method = get_operand_from_string(operand_2_string, instruction, &operand_2, 0);
    }

    if (is_valid_label(label)) {
        insertLabelNode(labelHead, label, *ic, CODE_LABEL);
    }

    printf("number of operands: %d\n", num_of_operands);



    if (num_of_operands == 0) {
        insertInstructionCodeWord(codeHead, 0, instruction.opcode, 0);
    } else if (num_of_operands == 1) {
        printf("operand_1_address_method: %d\n", operand_1_address_method);
        if (operand_1_address_method == REGISTER_DIRECT) {
            insertInstructionCodeWord(codeHead, 0, instruction.opcode, operand_1_address_method);
            insertRegisterCodeWord(codeHead, 0, operand_1.register_operand.registerNumber);
        } else if (operand_1_address_method == DIRECT) {
            insertInstructionCodeWord(codeHead, 0, instruction.opcode, operand_1_address_method);
            insertDataLabelCodeWord(codeHead, operand_1.label, ONE_ZERO);
        } else if (operand_1_address_method == IMMEDIATE) {
            insertInstructionCodeWord(codeHead, 0, instruction.opcode, operand_1_address_method);
            insertDataNumberCodeWord(codeHead, operand_1.number, ZERO);
        }
    } else {
        if (operand_1_address_method == REGISTER_DIRECT && operand_2_address_method == REGISTER_DIRECT) {
            insertInstructionCodeWord(codeHead, operand_1_address_method, instruction.opcode, operand_2_address_method);
            insertRegisterCodeWord(codeHead, operand_1.register_operand.registerNumber, operand_2.register_operand.registerNumber);
        } else if (operand_1_address_method == REGISTER_DIRECT && operand_2_address_method == DIRECT) {
            insertInstructionCodeWord(codeHead, operand_1_address_method, instruction.opcode, operand_2_address_method);
            insertRegisterCodeWord(codeHead, operand_1.register_operand.registerNumber, 0);
            insertDataLabelCodeWord(codeHead, operand_2.label, ONE_ZERO);
        } else if (operand_1_address_method == REGISTER_DIRECT && operand_2_address_method == IMMEDIATE) {
            insertInstructionCodeWord(codeHead, operand_1_address_method, instruction.opcode, operand_2_address_method);
            insertRegisterCodeWord(codeHead, operand_1.register_operand.registerNumber, 0);
            insertDataNumberCodeWord(codeHead, operand_2.number, ZERO);
        } else if (operand_1_address_method == DIRECT && operand_2_address_method == REGISTER_DIRECT) {
            insertInstructionCodeWord(codeHead, operand_1_address_method, instruction.opcode, operand_2_address_method);
            insertRegisterCodeWord(codeHead, 0, operand_2.register_operand.registerNumber);
            insertDataLabelCodeWord(codeHead, operand_1.label, ONE_ZERO);
        } else if (operand_1_address_method == DIRECT && operand_2_address_method == DIRECT) {
            insertInstructionCodeWord(codeHead, operand_1_address_method, instruction.opcode, operand_2_address_method);
            insertDataLabelCodeWord(codeHead, operand_1.label, ONE_ZERO);
            insertDataLabelCodeWord(codeHead, operand_2.label, ONE_ZERO);
        } else if (operand_1_address_method == DIRECT && operand_2_address_method == IMMEDIATE) {
            insertInstructionCodeWord(codeHead, operand_1_address_method, instruction.opcode, operand_2_address_method);
            insertDataLabelCodeWord(codeHead, operand_1.label, ONE_ZERO);
            insertDataNumberCodeWord(codeHead, operand_2.number, ZERO);
        } else if (operand_1_address_method == IMMEDIATE && operand_2_address_method == REGISTER_DIRECT) {
            insertInstructionCodeWord(codeHead, operand_1_address_method, instruction.opcode, operand_2_address_method);
            insertRegisterCodeWord(codeHead, 0, operand_2.register_operand.registerNumber);
            insertDataNumberCodeWord(codeHead, operand_1.number, ZERO);
        } else if (operand_1_address_method == IMMEDIATE && operand_2_address_method == DIRECT) {
            insertInstructionCodeWord(codeHead, operand_1_address_method, instruction.opcode, operand_2_address_method);
            insertDataNumberCodeWord(codeHead, operand_1.number, ZERO);
            insertDataLabelCodeWord(codeHead, operand_2.label, ONE_ZERO);
        } else if (operand_1_address_method == IMMEDIATE && operand_2_address_method == IMMEDIATE) {
            insertInstructionCodeWord(codeHead, operand_1_address_method, instruction.opcode, operand_2_address_method);
            insertDataNumberCodeWord(codeHead, operand_1.number, ZERO);
            insertDataNumberCodeWord(codeHead, operand_2.number, ZERO);
        }

    }
    (*ic)++;
    return 1;
}

int find_label(char *line, char *label, int *line_index) {
    if (!strchr(line, ':') !=
        NULL) { /* strchr returns a pointer to the first occurence of ":" in the string, if it is not in the string it returns NULL */
        return 0;
    }

    sscanf(line, "%[^:]", label); /* read until ":" the [^:] means read until ":" */
    label[strlen(label)] = '\0'; /* add the null terminator to the label */
    /* move the line pointer to the next character after the label */
    remove_white_spaces(label, 0);
    *line_index += strlen(label) + 1;

    /* check if the label is valid */
    if (is_valid_label(label)) {
        return 1;
    }
    return 0;
}


int is_directive(const char *line, const int *line_index) {
    if (line[*line_index] == '.') {
        return 1;
    }
    return 0;
}

int
handle_directive(char *line, int *line_index, long *ic, long *dc, DataWord *data_img, Label **labelHead, char *label) {
    /* extract directive name */
    int directive = find_directive_by_name(&line[*line_index]);
    if (directive == DIRECTIVE_NOT_FOUND) {
        printf("Error: directive not found\n");
        return -1;
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
handle_string_directive(const char *line, int *line_index, long *ic, long *dc, DataWord *data_img, Label **labelHead,
                        char *label) {
    /* move the line_index 6 characters forward to skip the "string" */
    *line_index += 6;
    SKIP_WHITE_SPACES(line, *line_index);
    /* check if the next character is a double quote */
    if (line[*line_index] != '"') {
        printf("Error: string directive must be followed by a string\n");
        return -1;
    }
    (*line_index)++;

    int temp_index = *line_index;
    /* check there is closing double quote */
    while (line[temp_index] != '"') {
        if (line[temp_index] == '\0') {
            printf("Error: string directive must be followed by a string\n");
            return -1;
        }
        temp_index++;
    }

    insertLabelNode(labelHead, label, *dc, DATA_LABEL);

    /* go until the next double quote and add each character to the data image */
    while (line[*line_index] != '"') {
        data_img[*dc].datatype = STRING;
        data_img[*dc].string[0] = line[*line_index];
        data_img[*dc].string[1] = '\0';
        (*dc)++;
        (*line_index)++;
    }
    (*line_index)++;
}

int handle_entry_or_extern_directive(const char *line, int *line_index, long *ic, long *dc, DataWord *data_img,
                                     Label **labelHead, char *label) {
    char extern_label[MAX_LABEL_SIZE];
    extern_label[0] = '\0';
    SKIP_WHITE_SPACES(line, *line_index);
    get_label_from_string(&line[*line_index], extern_label);
    insertLabelNode(labelHead, extern_label, 0, EXTERN_LABEL);
}

int handle_data_directive(const char *line, int *line_index, long *ic, long *dc, DataWord *data_img, Label **labelHead,
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
            data_img[*dc].number = atoi(temp_num);
            (*dc)++;
            continue;
        }
        temp_index++;
    }
    if (*dc - start_dc != 0) {
        insertLabelNode(labelHead, label, start_dc, DATA_LABEL);
    }
}
