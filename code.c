#include <string.h>

#include "helpers.h"
#include "tables.h"
#include "code.h"

/* directives mapping */
Directive directives[] = {
        {"data",   DATA},
        {"string", STRING},
        {"entry",  ENTRY},
        {"extern", EXTERN},
        {"",       DIRECTIVE_NOT_FOUND}
};

Directive find_directive_by_name(char *line) {
    int i;
    for (i = 0; i < sizeof(directives) / sizeof(Directive); ++i) {
        if (strncmp(directives[i].name, line, strlen(directives[i].name)) == 0) {
            return directives[i];
        }
    }
    return directives[4];
}

/* registers mapping */
Register registers[] = {
        {"@r0", R0},
        {"@r1", R1},
        {"@r2", R2},
        {"@r3", R3},
        {"@r4", R4},
        {"@r5", R5},
        {"@r6", R6},
        {"@r7", R7},
        {"",    REGISTER_NOT_FOUND}
};

Register findRegisterByName(const char *name) {
    int i;
    int j;
    for (i = 0; i < sizeof(registers) / sizeof(Register); ++i) {
        for (j = 0; j < strlen(registers[i].name); ++j) {
            if (name[j] != registers[i].name[j]) {
                break;
            }
            if (j == strlen(registers[i].name) - 1) {
                return registers[i];
            }
        }
    }
    return registers[8];
}

int mov_source[] = {IMMEDIATE, DIRECT, REGISTER_DIRECT};
int mov_dest[] = {DIRECT, REGISTER_DIRECT};

int cmp_source[] = {IMMEDIATE, DIRECT, REGISTER_DIRECT};
int cmp_dest[] = {IMMEDIATE, DIRECT, REGISTER_DIRECT};

int add_source[] = {IMMEDIATE, DIRECT, REGISTER_DIRECT};
int add_dest[] = {DIRECT, REGISTER_DIRECT};

int sub_source[] = {IMMEDIATE, DIRECT, REGISTER_DIRECT};
int sub_dest[] = {DIRECT, REGISTER_DIRECT};

int not_source[] = {NOT_EXISTS};
int not_dest[] = {DIRECT, REGISTER_DIRECT};

int clr_source[] = {NOT_EXISTS};
int clr_dest[] = {DIRECT, REGISTER_DIRECT};

int lea_source[] = {DIRECT};
int lea_dest[] = {DIRECT, REGISTER_DIRECT};

int inc_source[] = {NOT_EXISTS};
int inc_dest[] = {DIRECT, REGISTER_DIRECT};

int dec_source[] = {NOT_EXISTS};
int dec_dest[] = {DIRECT, REGISTER_DIRECT};

int jmp_source[] = {NOT_EXISTS};
int jmp_dest[] = {DIRECT, REGISTER_DIRECT};

int bne_source[] = {NOT_EXISTS};
int bne_dest[] = {DIRECT, REGISTER_DIRECT};

int red_source[] = {NOT_EXISTS};
int red_dest[] = {DIRECT, REGISTER_DIRECT};

int prn_source[] = {NOT_EXISTS};
int prn_dest[] = {IMMEDIATE, DIRECT, REGISTER_DIRECT};

int jsr_source[] = {NOT_EXISTS};
int jsr_dest[] = {DIRECT, REGISTER_DIRECT};

int rts_source[] = {NOT_EXISTS};
int rts_dest[] = {NOT_EXISTS};

int stop_source[] = {NOT_EXISTS};
int stop_dest[] = {NOT_EXISTS};

int not_found_source[] = {NOT_EXISTS};
int not_found_dest[] = {NOT_EXISTS};

/* instructions mapping */
Instruction instructions[] = {
        {"mov",  MOV,                   mov_source,       mov_dest,       3, 2, 2},
        {"cmp",  CMP,                   cmp_source,       cmp_dest,       3, 3, 2},
        {"add",  ADD,                   add_source,       add_dest,       3, 2, 2},
        {"sub",  SUB,                   sub_source,       sub_dest,       3, 2, 2},
        {"not",  NOT,                   not_source,       not_dest,       0, 2, 1},
        {"clr",  CLR,                   clr_source,       clr_dest,       0, 2, 1},
        {"lea",  LEA,                   lea_source,       lea_dest,       1, 2, 2},
        {"inc",  INC,                   inc_source,       inc_dest,       0, 2, 1},
        {"dec",  DEC,                   dec_source,       dec_dest,       0, 2, 1},
        {"jmp",  JMP,                   jmp_source,       jmp_dest,       0, 2, 1},
        {"bne",  BNE,                   bne_source,       bne_dest,       0, 2, 1},
        {"red",  RED,                   red_source,       red_dest,       0, 2, 1},
        {"prn",  PRN,                   prn_source,       prn_dest,       0, 3, 1},
        {"jsr",  JSR,                   jsr_source,       jsr_dest,       0, 2, 1},
        {"rts",  RTS,                   rts_source,       rts_dest,       0, 0, 0},
        {"stop", STOP,                  stop_source,      stop_dest,      0, 0, 0},
        {"",     INSTRUCTION_NOT_FOUND, not_found_source, not_found_dest, 0, 0, 0}
};

Instruction findInstructionByName(const char *name) {
    int i, j;

    for (i = 0; i < sizeof(instructions) / sizeof(Instruction); ++i) {
        for (j = 0; j < strlen(instructions[i].name); ++j) {
            if (name[j] != instructions[i].name[j]) {
                break;
            }
            if (j == strlen(instructions[i].name) - 1) {
                return instructions[i];
            }
        }
    }
    return instructions[16];
}

int is_instruction(char *line) {
    Instruction instruction = findInstructionByName(line);
    return instruction.opcode != INSTRUCTION_NOT_FOUND;
}


FuncResult get_operand_from_string(char *string, Instruction instruction, Operand *operand, int isSourceOperand) {
    FuncResult func_result;
    int i;
    int *addressingMethods;
    int numberOfAddressingMethods;
    func_result.result = 1;
    if (isSourceOperand) {
        addressingMethods = instruction.allowedSourceAddressingMethods;
        numberOfAddressingMethods = instruction.allowedSourceAddressingMethodsCount;
    } else {
        addressingMethods = instruction.allowedDestAddressingMethods;
        numberOfAddressingMethods = instruction.allowedDestAddressingMethodsCount;
    }

    for (i = 0; i < numberOfAddressingMethods; ++i) {
        if (addressingMethods[i] == NOT_EXISTS) {
            func_result.result = NOT_EXISTS;
            return func_result;
        }

        if (addressingMethods[i] == IMMEDIATE) {
            int number_operand;
            FuncResult result = get_number_from_string(string, &number_operand);
            if (result.result) {
                if (!is10BitsSigned(number_operand)) {
                    func_result.result = NOT_EXISTS;
                    strcpy((char *) func_result.message, "Number is not in 10 bits range");
                    return func_result;
                }
                operand->NameLabelUnion.number = number_operand;
                operand->operandType = NUMBER_OPERAND;
                operand->addressingMethod = IMMEDIATE;
                return func_result;
            }
        }

        if (addressingMethods[i] == DIRECT) {
            char label_operand[MAX_LINE_LENGTH];
            if (get_label_from_string(string, label_operand)) {
                FuncResult is_valid_label_result;
                remove_white_spaces(label_operand, 0);
                is_valid_label_result = is_valid_label(label_operand);
                if(!is_valid_label_result.result) {
                    return is_valid_label_result;
                }
                strcpy(operand->NameLabelUnion.label, label_operand);
                operand->addressingMethod = DIRECT;
                return func_result;
            }
        }

        if (addressingMethods[i] == REGISTER_DIRECT) {
            Register register_operand = findRegisterByName(string);
            if (register_operand.registerNumber != -1) {
                operand->NameLabelUnion.register_operand = register_operand;
                operand->operandType = REGISTER_OPERAND;
                operand->addressingMethod = REGISTER_DIRECT;
                return func_result;
            } else {
                func_result.result = NOT_EXISTS;
                strcpy((char *) func_result.message, "Invalid register");
                return func_result;
            }
        }
    }

    func_result.result = NOT_EXISTS;
    return func_result;
}


int is_directive(const char *line, const unsigned long *line_index) {
    if (line[*line_index] == '.') {
        return 1;
    }
    return 0;
}

int write_code_word(CodeWord **codeHead, Instruction instruction, const unsigned int *ic, int number_of_operands, Operand operand_1, Operand operand_2) {
    int l = 0;
    if (number_of_operands == 0) {
        l += 1;
        insert_instruction_code_word(codeHead, 0, instruction.opcode, 0, l, *ic, line_address);
        line_address++;
    } else if (number_of_operands == 1) {
        if (operand_1.addressingMethod == REGISTER_DIRECT) {
            l += 2;
            insert_instruction_code_word(codeHead, 0, instruction.opcode, operand_1.addressingMethod, l, *ic,
                                         line_address);
            line_address++;
            insert_register_code_word(codeHead, 0, operand_1.NameLabelUnion.register_operand.registerNumber,
                                      line_address);
            line_address++;
        } else if (operand_1.addressingMethod == DIRECT) {
            l += 2;
            insert_instruction_code_word(codeHead, 0, instruction.opcode, operand_1.addressingMethod, l, *ic,
                                         line_address);
            line_address++;
            insert_data_label_code_word(codeHead, operand_1.NameLabelUnion.label, RELOCATABLE, line_address);
            line_address++;
        } else if (operand_1.addressingMethod == IMMEDIATE) {
            l += 2;
            insert_instruction_code_word(codeHead, 0, instruction.opcode, operand_1.addressingMethod, l, *ic,
                                         line_address);
            line_address++;
            insert_data_number_code_word(codeHead, operand_1.NameLabelUnion.number, ABSOLUTE, line_address);
            line_address++;
        }
    } else {
        if (operand_1.addressingMethod == REGISTER_DIRECT && operand_2.addressingMethod == REGISTER_DIRECT) {
            l += 2;
            insert_instruction_code_word(codeHead, operand_1.addressingMethod, instruction.opcode,
                                         operand_2.addressingMethod, l, *ic,
                                         line_address);
            line_address++;
            insert_register_code_word(codeHead, operand_1.NameLabelUnion.register_operand.registerNumber,
                                      operand_2.NameLabelUnion.register_operand.registerNumber, line_address);
            line_address++;
        } else if (operand_1.addressingMethod == REGISTER_DIRECT && operand_2.addressingMethod == DIRECT) {
            l += 3;
            insert_instruction_code_word(codeHead, operand_1.addressingMethod, instruction.opcode,
                                         operand_2.addressingMethod, l, *ic,
                                         line_address);
            line_address++;
            insert_register_code_word(codeHead, operand_1.NameLabelUnion.register_operand.registerNumber, 0,
                                      line_address);
            line_address++;
            insert_data_label_code_word(codeHead, operand_2.NameLabelUnion.label, RELOCATABLE, line_address);
            line_address++;
        } else if (operand_1.addressingMethod == REGISTER_DIRECT && operand_2.addressingMethod == IMMEDIATE) {
            l += 3;
            insert_instruction_code_word(codeHead, operand_1.addressingMethod, instruction.opcode,
                                         operand_2.addressingMethod, l, *ic,
                                         line_address);
            line_address++;
            insert_register_code_word(codeHead, operand_1.NameLabelUnion.register_operand.registerNumber, 0,
                                      line_address);
            line_address++;
            insert_data_number_code_word(codeHead, operand_2.NameLabelUnion.number, ABSOLUTE, line_address);
            line_address++;
        } else if (operand_1.addressingMethod == DIRECT && operand_2.addressingMethod == REGISTER_DIRECT) {
            l += 3;
            insert_instruction_code_word(codeHead, operand_1.addressingMethod, instruction.opcode,
                                         operand_2.addressingMethod, l, *ic,
                                         line_address);
            line_address++;
            insert_register_code_word(codeHead, 0, operand_2.NameLabelUnion.register_operand.registerNumber,
                                      line_address);
            line_address++;
            insert_data_label_code_word(codeHead, operand_1.NameLabelUnion.label, RELOCATABLE, line_address);
            line_address++;
        } else if (operand_1.addressingMethod == DIRECT && operand_2.addressingMethod == DIRECT) {
            l += 3;
            insert_instruction_code_word(codeHead, operand_1.addressingMethod, instruction.opcode,
                                         operand_2.addressingMethod, l, *ic,
                                         line_address);
            line_address++;
            insert_data_label_code_word(codeHead, operand_1.NameLabelUnion.label, RELOCATABLE, line_address);
            line_address++;
            insert_data_label_code_word(codeHead, operand_2.NameLabelUnion.label, RELOCATABLE, line_address);
            line_address++;
        } else if (operand_1.addressingMethod == DIRECT && operand_2.addressingMethod == IMMEDIATE) {
            l += 3;
            insert_instruction_code_word(codeHead, operand_1.addressingMethod, instruction.opcode,
                                         operand_2.addressingMethod, l, *ic,
                                         line_address);
            line_address++;
            insert_data_label_code_word(codeHead, operand_1.NameLabelUnion.label, RELOCATABLE, line_address);
            line_address++;
            insert_data_number_code_word(codeHead, operand_2.NameLabelUnion.number, ABSOLUTE, line_address);
            line_address++;
        } else if (operand_1.addressingMethod == IMMEDIATE && operand_2.addressingMethod == REGISTER_DIRECT) {
            l += 3;
            insert_instruction_code_word(codeHead, operand_1.addressingMethod, instruction.opcode,
                                         operand_2.addressingMethod, l, *ic,
                                         line_address);
            line_address++;
            insert_register_code_word(codeHead, 0, operand_2.NameLabelUnion.register_operand.registerNumber,
                                      line_address);
            line_address++;
            insert_data_number_code_word(codeHead, operand_1.NameLabelUnion.number, ABSOLUTE, line_address);
            line_address++;
        } else if (operand_1.addressingMethod == IMMEDIATE && operand_2.addressingMethod == DIRECT) {
            l += 3;
            insert_instruction_code_word(codeHead, operand_1.addressingMethod, instruction.opcode,
                                         operand_2.addressingMethod, l, *ic,
                                         line_address);
            line_address++;
            insert_data_number_code_word(codeHead, operand_1.NameLabelUnion.number, ABSOLUTE, line_address);
            line_address++;
            insert_data_label_code_word(codeHead, operand_2.NameLabelUnion.label, RELOCATABLE, line_address);
            line_address++;
        } else if (operand_1.addressingMethod == IMMEDIATE && operand_2.addressingMethod == IMMEDIATE) {
            l += 3;
            insert_instruction_code_word(codeHead, operand_1.addressingMethod, instruction.opcode,
                                         operand_2.addressingMethod, l, *ic,
                                         line_address);
            line_address++;
            insert_data_number_code_word(codeHead, operand_1.NameLabelUnion.number, ABSOLUTE, line_address);
            line_address++;
            insert_data_number_code_word(codeHead, operand_2.NameLabelUnion.number, ABSOLUTE, line_address);
            line_address++;
        }
    }
    return l;
}