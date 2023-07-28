#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "helpers.h"

int is_empty_line(char *s) {
    while (*s != '\0') {
        if (!isspace(*s)) {
            return 0;
        }
        s++;
    }
    return 1;
}

void remove_white_spaces(char *str, int start_index) {
    int i = start_index;
    int j = 0;
    while (str[i] != '\0') {
        if (str[i] != ' ' && str[i] != '\t') {
            str[j] = str[i];
            ++j;
        }
        ++i;
    }
    str[j] = '\0';
}

void move_string_pointer_until_no_white_space(char **string) {
    while (**string && (**string == '\t' || **string == ' ')) {
        ++(*string);
    }
}

Directive directives[] = {
        {"data",   DATA},
        {"string", STRING},
        {"entry",  ENTRY},
        {"extern", EXTERN}
};


/**
 * @brief find the directive by name, gets a string that in the beginning of it there is a directive name
 * @param line
 * @return int
 */
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
        {"",   REGISTER_NOT_FOUND}
};

/**
 * @brief checks if the string is a valid number
 * @param line
 * @return int
 */
int is_valid_number(const char *str) {
    if (*str == '-' || *str == '+') {
        ++str;
    }
    while (*str != '\0') {

        printf("char: %c\n", *str);
        if (!isdigit(*str)) {
            return 0;
        }
        str++;
    }
    return 1;
}

/* Function to create a new node for the linked list */
Label *createLabelNode(const char *name, long address, enum e_label_type type) {
    Label *newNode = (Label *) malloc(sizeof(Label));
    if (newNode == NULL) {
        perror("Memory allocation failed.");
        exit(EXIT_FAILURE);
    }
    newNode->name = malloc(strlen(name) + 1);
    if (newNode->name == NULL) {
        printf("Memory allocation failed.");
    }
    strcpy(newNode->name, name);
    newNode->address = address;
    newNode->type = type;
    newNode->next = NULL;
    return newNode;
}

/* Function to insert a new node at the end of the linked list */
void insertLabelNode(Label **head, const char *name, long address, enum e_label_type type) {
    Label *newNode = createLabelNode(name, address, type);
    if (*head == NULL) {
        *head = newNode;
    } else {
        Label *current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
}

/* Function to print the linked list */
void printLabelList(Label **head) {
    Label *current = *head;
    while (current != NULL) {
        printf("Name: %s, Address: %ld, Type: %d\n", current->name, current->address, current->type);
        current = current->next;
    }
}

void freeLabelList(Label **head) {
    Label *current = *head;
    while (current != NULL) {
        Label *temp = current;
        current = current->next;
        free(temp->name);
        free(temp);
    }
}

Label *findLabelByNameAndType(Label **head, const char *name, enum e_label_type type) {
    Label *current = *head;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0 && current->type == type) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

enum e_address mov_source[] = {IMMEDIATE, DIRECT, REGISTER_DIRECT};
enum e_address mov_dest[] = {DIRECT, REGISTER_DIRECT};

enum e_address cmp_source[] = {IMMEDIATE, DIRECT, REGISTER_DIRECT};
enum e_address cmp_dest[] = {IMMEDIATE, DIRECT, REGISTER_DIRECT};

enum e_address add_source[] = {IMMEDIATE, DIRECT, REGISTER_DIRECT};
enum e_address add_dest[] = {DIRECT, REGISTER_DIRECT};

enum e_address sub_source[] = {IMMEDIATE, DIRECT, REGISTER_DIRECT};
enum e_address sub_dest[] = {DIRECT, REGISTER_DIRECT};

enum e_address not_source[] = {NOT_EXISTS};
enum e_address not_dest[] = {DIRECT, REGISTER_DIRECT};

enum e_address clr_source[] = {NOT_EXISTS};
enum e_address clr_dest[] = {DIRECT, REGISTER_DIRECT};

enum e_address lea_source[] = {DIRECT};
enum e_address lea_dest[] = {DIRECT, REGISTER_DIRECT};

enum e_address inc_source[] = {NOT_EXISTS};
enum e_address inc_dest[] = {DIRECT, REGISTER_DIRECT};

enum e_address dec_source[] = {NOT_EXISTS};
enum e_address dec_dest[] = {DIRECT, REGISTER_DIRECT};

enum e_address jmp_source[] = {NOT_EXISTS};
enum e_address jmp_dest[] = {DIRECT, REGISTER_DIRECT};

enum e_address bne_source[] = {NOT_EXISTS};
enum e_address bne_dest[] = {DIRECT, REGISTER_DIRECT};

enum e_address red_source[] = {NOT_EXISTS};
enum e_address red_dest[] = {DIRECT, REGISTER_DIRECT};

enum e_address prn_source[] = {NOT_EXISTS};
enum e_address prn_dest[] = {IMMEDIATE, DIRECT, REGISTER_DIRECT};

enum e_address jsr_source[] = {NOT_EXISTS};
enum e_address jsr_dest[] = {DIRECT, REGISTER_DIRECT};

enum e_address rts_source[] = {NOT_EXISTS};
enum e_address rts_dest[] = {NOT_EXISTS};

enum e_address stop_source[] = {NOT_EXISTS};
enum e_address stop_dest[] = {NOT_EXISTS};

enum e_address not_found_source[] = {NOT_EXISTS};
enum e_address not_found_dest[] = {NOT_EXISTS};

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


/* Function to create a new node for the codeWord linked list */

CodeWord *createCodeWordNode(enum e_code_word_type codeWordType) {
    CodeWord *newNode = (CodeWord *) malloc(sizeof(CodeWord));
    if (newNode == NULL) {
        perror("Memory allocation failed.");
        exit(EXIT_FAILURE);
    }
    newNode->codeWordType = codeWordType;
    newNode->next = NULL;

    return newNode;
}

void insertInstructionCodeWord(CodeWord **head, enum e_address source, unsigned int opcode, enum e_address dest) {
    CodeWord *newNode = createCodeWordNode(INSTRUCTION_WORD);
    if (newNode != NULL) {
        newNode->instruction.are = ZERO;
        newNode->instruction.dest = dest;
        newNode->instruction.opcode = opcode;
        newNode->instruction.source = source;

        if (*head == NULL) {
            *head = newNode;
        } else {
            CodeWord *current = *head;
            while (current->next != NULL) {
                current = current->next;
            }
            current->next = newNode;
        }
    }
}

void insertRegisterCodeWord(CodeWord **head, enum e_registers source_register,
                            enum e_registers dest_register) {
    CodeWord *newNode = createCodeWordNode(REGISTER_WORD);
    if (newNode != NULL) {
        newNode->registerWord.are = ZERO;
        newNode->registerWord.source = source_register;
        newNode->registerWord.dest = dest_register;

        if (*head == NULL) {
            *head = newNode;
        } else {
            CodeWord *current = *head;
            while (current->next != NULL) {
                current = current->next;
            }
            current->next = newNode;
        }
    }
}

void insertDataNumberCodeWord(CodeWord **head, signed int value, enum e_are are) {
    CodeWord *newNode = createCodeWordNode(DATA_NUMBER_WORD);
    if (newNode != NULL) {
        newNode->data.are = are;
        newNode->data.value = value;

        if (*head == NULL) {
            *head = newNode;
        } else {
            CodeWord *current = *head;
            while (current->next != NULL) {
                current = current->next;
            }
            current->next = newNode;
        }
    }
}

void insertDataLabelCodeWord(CodeWord **head, char *label, enum e_are are) {
    CodeWord *newNode = createCodeWordNode(DATA_LABEL_WORD);
    if (newNode != NULL) {
        newNode->data.are = are;
        newNode->data.label = malloc(strlen(label) + 1);
        if (newNode->data.label == NULL) {
            printf("Memory allocation failed.");
        }
        strcpy(newNode->data.label, label);

        if (*head == NULL) {
            *head = newNode;
        } else {
            CodeWord *current = *head;
            while (current->next != NULL) {
                current = current->next;
            }
            current->next = newNode;
        }
    }
}

void printCodeWordList(CodeWord **head) {
    CodeWord *current = *head;
    while (current != NULL) {
        if (current->codeWordType == INSTRUCTION_WORD) {
            printf("Instruction - ARE: %u, dest: %u, opcode: %u, source: %u\n",
                   current->instruction.are, current->instruction.dest, current->instruction.opcode,
                   current->instruction.source);
        } else if (current->codeWordType == REGISTER_WORD) {
            printf("Register - ARE: %u, dest: %u, source: %u\n",
                   current->registerWord.are, current->registerWord.dest, current->registerWord.source);
        } else if(current->codeWordType == DATA_LABEL_WORD) {
            printf("Data - Label: %s, ARE: %d\n", current->data.label, current->data.are);
        }else {
            printf("Data - Value: %d, ARE: %d\n", current->data.value, current->data.are);
        }
        current = current->next;
    }
}

void freeCodeWordList(CodeWord **head) {
    CodeWord *current = *head;
    while (current != NULL) {
        CodeWord *temp = current;
        current = current->next;
        free(temp);
    }
    *head = NULL;
}

/**
 * @brief gets a string that it's first character is a number and returns the number
 * @param string
 * @return int
 */
int get_number_from_string(char *string, int *number) {
    int i = 0;
    char temp_num[MAX_LINE_LENGTH];
    temp_num[0] = '\0';
    if (string[i] == '-') {
        temp_num[0] = '-';
        i++;
    }


    if (isdigit(string[i])) {
        while (string[i] != '\0' && string[i] != ' ' && string[i] != '\t' && string[i] != '\n') {
            temp_num[i] = string[i];
            i++;
        }
        temp_num[i] = '\0';

        /* check if the number found */

        if (!is_valid_number(temp_num)) {
            printf("Error: invalid number\n");
            return -1;
        }

        *number = atoi(temp_num);

        return 1;
    }

    return 0;

}


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

int is_valid_label(const char *label) {
    size_t len = strlen(label);

    /* Check the length of the label (must be at most 31 characters) */
    if (len == 0 || len > 31) {
        return 0; /* false */
    }

    /* Check if the label starts with an alphabetic character */
    if (!isalpha(label[0])) {
        return 0; /* false */
    }

    /* Check if the remaining characters are valid label characters */
    int i;
    for (i = 1; i < len; ++i) {
        /* Check if the character is an alphanumeric character */
        if (!isalnum(label[i])) {
            return 0; /* false */
        }
    }

    return 1; /* true */
}


/**
 * @brief gets a string and returns the label in it
 * @param string
 * @param label
 * @return
 */
int get_label_from_string(char *string, char *label) {
    if (!isalpha(string[0])) {
        return 0;
    }
    int i = 0;
    while (string[i] != '\0' && string[i] != ' ' && string[i] != '\t' && string[i] != ':' && string[i] != '\n') {
        label[i] = string[i];
        i++;
    }
    label[i] = '\0';
    return is_valid_label(label);
}

int get_operand_from_string(char *string, Instruction instruction, Operand *operand, int isSourceOperand) {
    int i;
    enum e_address *addressingMethods;
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
                operand->number = number_operand;
                operand->operandType = NUMBER_O;
                return IMMEDIATE;
            }
        }

        if (addressingMethods[i] == DIRECT) {
            char label_operand[MAX_LINE_LENGTH];
            if (get_label_from_string(string, label_operand)) {
                if(is_valid_label(label_operand)) {
                    remove_white_spaces(label_operand, 0);
                    strcpy(operand->label, label_operand);
                    return DIRECT;
                } else {
                    printf("Error: invalid label\n");
                    return 0;
                }
            }
        }

        if (addressingMethods[i] == REGISTER_DIRECT) {
            Register register_operand = findRegisterByName(string);
            if (register_operand.registerNumber != -1) {
                operand->register_operand = register_operand;
                operand->operandType = REGISTER_O;
                return REGISTER_DIRECT;
            }
        }
    }

    return NOT_EXISTS;
}