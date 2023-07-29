#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "tables.h"


Label *createLabelNode(const char *name, long address, enum LabelType type) {
    Label *newNode = (Label *) malloc(sizeof(Label));
    if (newNode == NULL) {
        perror("Memory allocation failed.");
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

void insertLabelNode(Label **head, const char *name, long address, enum LabelType type) {
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

Label *findLabelByNameAndType(Label **head, const char *name, enum LabelType type) {
    Label *current = *head;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0 && current->type == type) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

CodeWord *createCodeWordNode(enum codeWordType codeWordType) {
    CodeWord *newNode = (CodeWord *) malloc(sizeof(CodeWord));
    if (newNode == NULL) {
        perror("Memory allocation failed.");
    }
    newNode->codeWordType = codeWordType;
    newNode->next = NULL;

    return newNode;
}

void insertInstructionCodeWord(CodeWord **head, enum AddressMethod source, unsigned int opcode, enum AddressMethod dest) {
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

void insertRegisterCodeWord(CodeWord **head, enum Register source_register,
                            enum Register dest_register) {
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

void insertDataNumberCodeWord(CodeWord **head, signed int value, enum Are are) {
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

void insertDataLabelCodeWord(CodeWord **head, char *label, enum Are are) {
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
    int number = 0;
    CodeWord *current = *head;
    while (current != NULL) {
        if (current->codeWordType == INSTRUCTION_WORD) {
            printf("Instruction - ARE: %u, source: %u, opcode: %u, dest: %u\n",
                   current->instruction.are, current->instruction.source, current->instruction.opcode,
                   current->instruction.dest);
        } else if (current->codeWordType == REGISTER_WORD) {
            printf("Register - ARE: %u, source: %u, dest: %u\n",
                   current->registerWord.are, current->registerWord.source, current->registerWord.dest);
        } else if (current->codeWordType == DATA_LABEL_WORD) {
            printf("Data - ARE: %d, Label: %s\n", current->data.are, current->data.label);
        } else {
            printf("Data - ARE: %d, Value: %d\n", current->data.are, current->data.value);
        }
        current = current->next;
        number++;
    }

    printf("number of code words: %d\n", number);

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
