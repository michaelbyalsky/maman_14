#ifndef MAMAN_14_HELPERS_H
#define MAMAN_14_HELPERS_H

#include "globals.h"

int get_number_from_string(char *string, int *number);

int is_valid_label(const char *label);

int get_label_from_string(char *string, char *label);

int get_operand_from_string(char *string, Instruction instruction, Operand *operand, int isSourceOperand);
void insertInstructionCodeWord(CodeWord **head, enum e_address source, unsigned int opcode, enum e_address dest);
void insertDataLabelCodeWord(CodeWord **head, char *label, enum e_are are);
void insertRegisterCodeWord(CodeWord **head, enum e_registers source_register,
                            enum e_registers dest_register);

void insertDataNumberCodeWord(CodeWord **head, signed int value, enum e_are are);

void printCodeWordList(CodeWord **head);

void freeCodeWordList(CodeWord **head);

Register findRegisterByName(const char *name);

int is_empty_line(char *s);

int find_directive_by_name(char *line);

Instruction findInstructionByName(const char *name);

int is_instruction(char *line);

void remove_white_spaces(char *str, int start_index);

void freeLabelList(Label **head);

int is_valid_number(const char *str);

Label *createLabelNode(const char *name, long address, enum e_label_type type);

void insertLabelNode(Label **head, const char *name, long address, enum e_label_type type);

void printLabelList(Label **head);

#define SKIP_WHITE_SPACES(string, index) \
    while (string[(index)] && (string[(index)] == '\t' || string[(index)] == ' ')) { \
        ++(index); \
    }


#endif
