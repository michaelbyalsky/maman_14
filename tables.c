#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "tables.h"

DataWord *create_data_word_node(enum Directives dataType) {
    DataWord *newNode = (DataWord *) malloc(sizeof(DataWord));
    if (newNode == NULL) {
        perror("Memory allocation failed.");
    }
    newNode->datatype = dataType;
    newNode->next = NULL;

    return newNode;
}

void insert_number_data_word(DataWord **head, int number, int unsigned address) {
    DataWord *newNode = create_data_word_node(DATA);
    if (newNode != NULL) {
        newNode->NumberStringUnion.number = number;
        newNode->address = address;

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

void insert_string_data_word(DataWord **head, char *string, int unsigned address) {
    DataWord *newNode = create_data_word_node(STRING);
    if (newNode != NULL) {
        strcpy(newNode->NumberStringUnion.string, string);
        newNode->address = address;

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

void print_data_word_list(DataWord **head) {
    int counter = 0;
    DataWord *current = *head;
    while (current != NULL) {
        if (current->datatype == DATA) {
            printf("Data - Value: %d, address: %u\n", current->NumberStringUnion.number, current->address);
        } else {
            printf("Data - String: %s, address: %u\n", current->NumberStringUnion.string, current->address);
        }
        current = current->next;
        counter++;
    }
    printf("number of data words: %d\n", counter);
}

void free_data_word_list(DataWord **head) {
    DataWord *current = *head;
    while (current != NULL) {
        DataWord *temp = current;
        current = current->next;
        free(temp);
    }
    *head = NULL;
}


Label *create_label_node(const char *name, unsigned int address, enum LabelType type) {
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

void insert_label_node(Label **head, const char *name, unsigned int address, enum LabelType type) {
    Label *newNode = create_label_node(name, address, type);
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

void print_label_list(Label **head) {
    Label *current = *head;
    while (current != NULL) {
        printf("Name: %s, Address: %u, Type: %d\n", current->name, current->address, current->type);
        current = current->next;
    }
}

void free_label_list(Label **head) {
    Label *current = *head;
    while (current != NULL) {
        Label *temp = current;
        current = current->next;
        free(temp->name);
        free(temp);
    }
}

Label *find_label_by_name(Label **head, const char *name) {
    Label *current = *head;
    while (current != NULL) {
        if (strncmp(current->name, name, strlen(current->name)) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

int update_label_type(Label **head, const char *name, enum LabelType type) {
    Label *current = *head;
    while (current != NULL) {
        if (strncmp(current->name, name, strlen(current->name)) == 0) {
            current->type = type;
            return 1;
        }
        current = current->next;
    }
    return 0;
}

int label_exists(Label **head, const char *name) {
    Label *current = *head;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            return 1;
        }
        current = current->next;
    }
    return 0;
}


CodeWord *create_code_word_node(enum codeWordType codeWordType, int unsigned address) {
    CodeWord *newNode = (CodeWord *) malloc(sizeof(CodeWord));
    if (newNode == NULL) {
        perror("Memory allocation failed.");
    }
    newNode->codeWordType = codeWordType;
    newNode->next = NULL;
    newNode->address = address;

    return newNode;
}

void insert_instruction_code_word(CodeWord **head, enum AddressMethod source, unsigned int opcode, enum AddressMethod dest, int totalWords,
                               int ic, int unsigned address) {
    CodeWord *newNode = create_code_word_node(INSTRUCTION_WORD, address);
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

void insert_register_code_word(CodeWord **head, enum Register source_register,
                            enum Register dest_register, int unsigned address) {
    CodeWord *newNode = create_code_word_node(REGISTER_WORD, address);
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

void insert_data_number_code_word(CodeWord **head, signed int value, enum Are are, int unsigned address) {
    CodeWord *newNode = create_code_word_node(DATA_NUMBER_WORD, address);
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

void insert_data_label_code_word(CodeWord **head, char *label, enum Are are, int unsigned address) {
    CodeWord *newNode = create_code_word_node(DATA_LABEL_WORD, address);
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

void print_code_word_list(CodeWord **head) {
    int number = 0;
    CodeWord *current = *head;
    while (current != NULL) {
        if (current->codeWordType == INSTRUCTION_WORD) {
            printf("Instruction - ARE: %u, source: %u, opcode: %u, dest: %u, address: %u\n",
                   current->are, current->CodeWordUnion.instruction.source, current->CodeWordUnion.instruction.opcode,
                   current->CodeWordUnion.instruction.dest, current->address);
        } else if (current->codeWordType == REGISTER_WORD) {
            printf("Register - ARE: %u, source: %u, dest: %u, address: %u\n",
                   current->are, current->CodeWordUnion.registerWord.source, current->CodeWordUnion.registerWord.dest, current->address);
        } else if (current->codeWordType == DATA_LABEL_WORD) {
            printf("Data - ARE: %d, Label: %s, address: %u\n",
                   current->are, current->CodeWordUnion.data.label, current->address);
        }else if (current->codeWordType == DATA_ADDRESS_WORD) {
            printf("Data - ARE: %d, Label address: %u, address: %u\n",
                   current->are, current->CodeWordUnion.data.labelAddress, current->address);
        }  else {
            printf("Data - ARE: %d, Value: %d, address: %u\n",
                   current->are, current->CodeWordUnion.data.value, current->address);
        }
        current = current->next;
        number++;
    }

    printf("number of code words: %d\n", number);
}

CodeWord* find_code_word_by_ic(CodeWord **head, int ic) {
    CodeWord *current = *head;
    while (current != NULL) {
        if (current->CodeWordUnion.instruction.ic == ic) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}


void free_code_word_list(CodeWord **head) {
    CodeWord *current = *head;
    while (current != NULL) {
        CodeWord *temp = current;
        current = current->next;
        free(temp);
    }
    *head = NULL;
}


