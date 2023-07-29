#include <string.h>

#include "helpers.h"
#include "code.h"

Directive directives[] = {
        {"data",   DATA},
        {"string", STRING},
        {"entry",  ENTRY},
        {"extern", EXTERN}
};

int find_directive_by_name(char *line) {
    int i;
    for (i = 0; i < sizeof(directives) / sizeof(Directive); ++i) {
        if (strncmp(directives[i].name, line, strlen(directives[i].name)) == 0) {
            return directives[i].directive;
        }
    }
    return DIRECTIVE_NOT_FOUND;
}

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


int get_operand_from_string(char *string, Instruction instruction, Operand *operand, int isSourceOperand) {
    int i;
    int *addressingMethods;
    int numberOfAddressingMethods;
    if (isSourceOperand) {
        addressingMethods = instruction.allowedSourceAddressingMethods;
        numberOfAddressingMethods = instruction.allowedSourceAddressingMethodsCount;
    } else {
        addressingMethods = instruction.allowedDestAddressingMethods;
        numberOfAddressingMethods = instruction.allowedDestAddressingMethodsCount;
    }

    for (i = 0; i < numberOfAddressingMethods; ++i) {
        if (addressingMethods[i] == NOT_EXISTS) {
            return NOT_EXISTS;
        }

        if (addressingMethods[i] == IMMEDIATE) {
            int number_operand;
            if (get_number_from_string(string, &number_operand)) {
                operand->NameLabelUnion.number = number_operand;
                operand->operandType = NUMBER_O;
                return IMMEDIATE;
            }
        }

        if (addressingMethods[i] == DIRECT) {
            char label_operand[MAX_LINE_LENGTH];
            if (get_label_from_string(string, label_operand)) {
                remove_white_spaces(label_operand, 0);
                strcpy(operand->NameLabelUnion.label, label_operand);
                return DIRECT;
            }
        }

        if (addressingMethods[i] == REGISTER_DIRECT) {
            Register register_operand = findRegisterByName(string);
            if (register_operand.registerNumber != -1) {
                operand->NameLabelUnion.register_operand = register_operand;
                operand->operandType = REGISTER_O;
                return REGISTER_DIRECT;
            }
        }
    }

    return NOT_EXISTS;
}