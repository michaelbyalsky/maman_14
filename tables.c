#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "tables.h"

DataWord *createDataWordNode(enum Directives dataType) {
    DataWord *newNode = (DataWord *) malloc(sizeof(DataWord));
    if (newNode == NULL) {
        perror("Memory allocation failed.");
    }
    newNode->datatype = dataType;
    newNode->next = NULL;

    return newNode;
}

void insertNumberDataWord(DataWord **head, int number) {
    DataWord *newNode = createDataWordNode(DATA);
    if (newNode != NULL) {
        newNode->NumberStringUnion.number = number;

        if (*head == NULL) {
            *head = newNode;
        } else {
            DataWord *current = *head;
            while (current->next != NULL) {
                current = current->next;
            }
            current->next = newNode;
        }
    }
}

void insertStringDataWord(DataWord **head, char *string) {
    DataWord *newNode = createDataWordNode(STRING);
    if (newNode != NULL) {
        strcpy(newNode->NumberStringUnion.string, string);

        if (*head == NULL) {
            *head = newNode;
        } else {
            DataWord *current = *head;
            while (current->next != NULL) {
                current = current->next;
            }
            current->next = newNode;
        }
    }
}

void printDataWordList(DataWord **head) {
    int counter = 0;
    DataWord *current = *head;
    while (current != NULL) {
        if (current->datatype == DATA) {
            printf("Data - Value: %d\n", current->NumberStringUnion.number);
        } else {
            printf("Data - String: %s\n", current->NumberStringUnion.string);
        }
        current = current->next;
        counter++;
    }
    printf("number of data words: %d\n", counter);
}

void freeDataWordList(DataWord **head) {
    DataWord *current = *head;
    while (current != NULL) {
        DataWord *temp = current;
        current = current->next;
        free(temp);
    }
    *head = NULL;
}


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

Label *findLabelByName(Label **head, const char *name) {
    Label *current = *head;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

int updateLabelType(Label **head, const char *name, enum LabelType type) {
    Label *current = *head;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            current->type = type;
            return 1;
        }
        current = current->next;
    }
    return 0;
}

int labelExists(Label **head, const char *name) {
    Label *current = *head;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            return 1;
        }
        current = current->next;
    }
    return 0;
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

void insertInstructionCodeWord(CodeWord **head, enum AddressMethod source, unsigned int opcode, enum AddressMethod dest, int totalWords,
                               int ic) {
    CodeWord *newNode = createCodeWordNode(INSTRUCTION_WORD);
    if (newNode != NULL) {
        newNode->are = ZERO;
        newNode->CodeWordUnion.instruction.dest = dest;
        newNode->CodeWordUnion.instruction.opcode = opcode;
        newNode->CodeWordUnion.instruction.source = source;
        newNode->CodeWordUnion.instruction.totalWords = totalWords;
        newNode->CodeWordUnion.instruction.ic = ic;

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
        newNode->are = ZERO;
        newNode->CodeWordUnion.registerWord.source = source_register;
        newNode->CodeWordUnion.registerWord.dest = dest_register;

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
        newNode->are = are;
        newNode->CodeWordUnion.data.value = value;

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
        newNode->are = are;
        newNode->CodeWordUnion.data.label = malloc(strlen(label) + 1);
        if (newNode->CodeWordUnion.data.label == NULL) {
            printf("Memory allocation failed.");
        }
        strcpy(newNode->CodeWordUnion.data.label, label);

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
                   current->are, current->CodeWordUnion.instruction.source, current->CodeWordUnion.instruction.opcode,
                   current->CodeWordUnion.instruction.dest);
        } else if (current->codeWordType == REGISTER_WORD) {
            printf("Register - ARE: %u, source: %u, dest: %u\n",
                   current->are, current->CodeWordUnion.registerWord.source, current->CodeWordUnion.registerWord.dest);
        } else if (current->codeWordType == DATA_LABEL_WORD) {
            printf("Data - ARE: %d, Label: %s\n", current->are, current->CodeWordUnion.data.label);
        } else {
            printf("Data - ARE: %d, Value: %d\n", current->are, current->CodeWordUnion.data.value);
        }
        current = current->next;
        number++;
    }

    printf("number of code words: %d\n", number);
}

CodeWord* findCodeWordByIC(CodeWord **head, int ic) {
    CodeWord *current = *head;
    while (current != NULL) {
        if (current->CodeWordUnion.instruction.ic == ic) {
            return current;
        }
        current = current->next;
    }
    return NULL;
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


