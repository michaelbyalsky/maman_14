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
        {"r0", R0},
        {"r1", R1},
        {"r2", R2},
        {"r3", R3},
        {"r4", R4},
        {"r5", R5},
        {"r6", R6},
        {"r7", R7}
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
    while (*str) {
        if (!isdigit(*str)) {
            return 0;
        }
        ++str;
    }
    return 1;
}

/* Function to create a new node for the linked list */
Label *createLabelNode(const char *name, long address, enum e_label_type type) {
    Label *newNode = (Label *)malloc(sizeof(Label));
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


Instruction instructions[] = {
        {"mov",  MOV,  {IMMEDIATE, DIRECT, REGISTER_DIRECT}, {DIRECT,    REGISTER_DIRECT}},
        {"cmp",  CMP,  {IMMEDIATE, DIRECT, REGISTER_DIRECT}, {IMMEDIATE, DIRECT, REGISTER_DIRECT}},
        {"add",  ADD,  {IMMEDIATE, DIRECT, REGISTER_DIRECT}, {DIRECT,    REGISTER_DIRECT}},
        {"sub",  SUB,  {IMMEDIATE, DIRECT, REGISTER_DIRECT}, {DIRECT,    REGISTER_DIRECT}},
        {"not",  NOT,  {NOT_EXISTS},                         {DIRECT,    REGISTER_DIRECT}},
        {"clr",  CLR,  {NOT_EXISTS},                         {DIRECT,    REGISTER_DIRECT}},
        {"lea",  LEA,  {DIRECT},                             {DIRECT,    REGISTER_DIRECT}},
        {"inc",  INC,  {NOT_EXISTS},                         {DIRECT,    REGISTER_DIRECT}},
        {"dec",  DEC,  {NOT_EXISTS},                         {DIRECT,    REGISTER_DIRECT}},
        {"jmp",  JMP,  {NOT_EXISTS},                         {DIRECT,    REGISTER_DIRECT}},
        {"bne",  BNE,  {NOT_EXISTS},                         {DIRECT,    REGISTER_DIRECT}},
        {"red",  RED,  {NOT_EXISTS},                         {DIRECT,    REGISTER_DIRECT}},
        {"prn",  PRN,  {NOT_EXISTS},                         {IMMEDIATE, DIRECT, REGISTER_DIRECT}},
        {"jsr",  JSR,  {NOT_EXISTS},                         {DIRECT,    REGISTER_DIRECT}},
        {"rts",  RTS,  {NOT_EXISTS},                         {NOT_EXISTS}},
        {"stop", STOP, {NOT_EXISTS},                         {NOT_EXISTS}},
        {"",     INSTRUCTION_NOT_FOUND, {NOT_EXISTS},                         {NOT_EXISTS}}
};

Instruction findInstructionByName(const char *name) {
    int i, j;

    for (i = 0; i < sizeof(instructions) / sizeof(Instruction); ++i) {
        for(j = 0; j < strlen(instructions[i].name); ++j) {
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
CodeWord *createCodeWordNode(unsigned int ARE, unsigned int dest, unsigned int opcode, unsigned int source) {
    CodeWord *newNode = (CodeWord *)malloc(sizeof(CodeWord));
    if (newNode == NULL) {
        perror("Memory allocation failed.");
        exit(EXIT_FAILURE);
    }
    newNode->ARE = ARE;
    newNode->dest = dest;
    newNode->opcode = opcode;
    newNode->source = source;
    newNode->next = NULL;
    return newNode;
}

/* Function to insert a new node at the end of the codeWord linked list */
void insertCodeWordNode(CodeWord **head, unsigned int ARE, unsigned int dest, unsigned int opcode, unsigned int source) {
    CodeWord *newNode = createCodeWordNode(ARE, dest, opcode, source);
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

/* Function to print the codeWord linked list */
void printCodeWordList(CodeWord **head) {
    CodeWord *current = *head;
    while (current != NULL) {
        printf("ARE: %d, dest: %d, opcode: %d, source: %d\n", current->ARE, current->dest, current->opcode,
               current->source);
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
}