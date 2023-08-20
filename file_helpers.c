#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "tables.h"
#include "helpers.h"

const char base64Chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
const char *entriesSuffix = ".ent";
const char *externalsSuffix = ".ext";
const char *objectSuffix = ".ob";
char *entriesFileName;
char *externalsFileName;
char *objectFileName;
FILE *entries;
FILE *externals;
FILE *objectFile;

/**
 * @brief print code word list to file
 * @param outputFile  - output file
 * @param head - code word list
 * @param ic - instruction counter
 * @param dc - data counter
 */
void print_code_word_list(FILE *outputFile, CodeWord **head, unsigned int ic, unsigned int dc);

/**
 * @brief print data word list to file
 * @param outputFile - output file
 * @param head - data word list
 */
void print_data_word_list(FILE *outputFile, DataWord **head);

/**
 * @brief print external to file
 * @param name - external name
 * @param address - external address
 */
void print_external(char *name, unsigned int address);

/**
 * @brief print entries to file
 * @param labelHead - label list
 */
void print_entries(Label **labelHead);

/**
 * @brief print single value in base64 to file
 * @param outputFile - output file
 * @param value - value to print
 */
void print_base64(FILE *outputFile, unsigned int value);

void write_to_file(const char *filename, CodeWord **codeHead, DataWord **dataHead, Label **labelHead, unsigned int ic,
                   unsigned int dc) {
    externalsFileName = getNewFileName(filename, externalsSuffix);
    entriesFileName = getNewFileName(filename, entriesSuffix);
    objectFileName = getNewFileName(filename, objectSuffix);

    objectFile = fopen(objectFileName, "w");
    /* print the code word list, to avoid another iteration we will print also the externals if exists */
    print_code_word_list(objectFile, codeHead, ic, dc);
    /* print the data word list */
    print_data_word_list(objectFile, dataHead);
    fclose(objectFile);

    /* print the entries */
    print_entries(labelHead);

    free(objectFileName);
    free(entriesFileName);
    free(externalsFileName);
}

void print_base64(FILE *outputFile, unsigned int value) {
    fprintf(outputFile, "%c%c\n", base64Chars[(value >> 6) & 0x3F], base64Chars[value & 0x3F]);
}

void print_code_word_list(FILE *outputFile, CodeWord **head, unsigned int ic, unsigned int dc) {
    CodeWord *current = *head;
    int is_externals_exists = 0;
    fprintf(outputFile, "%d %d\n", ic, dc);

    while (current != NULL) {
        unsigned int binaryCode;
        if (current->codeWordType == INSTRUCTION_WORD) {
            /* convert the instruction to binary according to the instruction format */
            binaryCode =
                    ((current->CodeWordUnion.instruction.source & 0x7) << 9) |
                    ((current->CodeWordUnion.instruction.opcode & 0xF) << 5) |
                    ((current->CodeWordUnion.instruction.dest & 0x7) << 2) |
                    (current->are & 0x3);
            print_base64(outputFile, binaryCode);
        } else if (current->codeWordType == REGISTER_WORD) {
            /* convert the register extra word to binary according to the instruction format */
            binaryCode = ((current->CodeWordUnion.registerWord.source & 0x1F) << 7) |
                         ((current->CodeWordUnion.registerWord.dest & 0x1F) << 2) |
                         (current->are & 0x3);
            print_base64(outputFile, binaryCode);
        } else if (current->codeWordType == DATA_LABEL_WORD || current->codeWordType == DATA_ADDRESS_WORD) {
            if (current->are == ZERO_ONE) {
                if (!is_externals_exists) {
                    /* if this is the first external, open the file */
                    externals = fopen(externalsFileName, "w");
                    is_externals_exists = 1;
                }

                binaryCode = (current->are & 0x3);
                /* print the external to the externals file */
                print_external(current->CodeWordUnion.data.label, current->address);
            } else {
                binaryCode = ((current->CodeWordUnion.data.labelAddress & 0xFFF) << 2) | (current->are & 0x3);
            }
            print_base64(outputFile, binaryCode);
        } else if (current->codeWordType == DATA_NUMBER_WORD) {
            if (current->CodeWordUnion.data.value < 0) {
                /* convert the data number to binary according to the instruction format */
                binaryCode = (unsigned int) (((~abs(current->CodeWordUnion.data.value) + 1) & 0xFFF) << 2) |
                             (current->are & 0x3);
            } else {
                binaryCode = current->CodeWordUnion.data.value & 0xFFF;
            }
            print_base64(outputFile, binaryCode);
        }

        current = current->next;
    }
    if (is_externals_exists) {
        fclose(externals);
    }
}

void print_data_word_list(FILE *outputFile, DataWord **head) {
    DataWord *current = *head;
    while (current != NULL) {
        unsigned int binaryCode;
        if (current->datatype == DATA) {
            /* if the data is negative, convert it to binary according to the instruction format */
            if (current->NumberStringUnion.number < 0) {
                binaryCode = (unsigned int) (((~abs(current->NumberStringUnion.number) + 1) & 0xFFF));
            } else {
                /* convert the data number to binary according to the instruction format */
                binaryCode = current->NumberStringUnion.number & 0xFFF;
            }
        } else {
            binaryCode = (unsigned int) current->NumberStringUnion.string[0];
        }

        print_base64(outputFile, binaryCode);

        current = current->next;
    }
}

void print_external(char *name, unsigned int address) {
    remove_new_line_char_from_string(name);
    fprintf(externals, "%s %d\n", name, address);
}

void print_entries(Label **labelHead) {
    int is_entries_exists = 0;

    Label *current = *labelHead;
    while (current != NULL) {
        if (current->type == ENTRY_LABEL) {
            /* if this is the first entry, open the file */
            if (!is_entries_exists) {
                entries = fopen(entriesFileName, "w");
                is_entries_exists = 1;
            }

            fprintf(entries, "%s %d\n", current->name, current->address);
        }

        current = current->next;
    }
    if (is_entries_exists) {
        fclose(entries);
    }
}
