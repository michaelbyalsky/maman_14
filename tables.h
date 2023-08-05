#ifndef MAMAN_14_TABLES_H
#define MAMAN_14_TABLES_H

#include "globals.h"
#include "code.h"
#include "tables.h"

/**
 * @brief Creates a new instruction code word
 * @param source - the source operand
 * @param opcode - the opcode of the instruction
 * @param dest - the destination operand
 * @return
 */
void insertInstructionCodeWord(CodeWord **head, enum AddressMethod source, unsigned int opcode, enum AddressMethod dest);


/**
 * @brief Inserts a data label code word to the end of the list
 * @param head - the head of the list
 * @param label - the label to insert
 * @param are - the are of the label
 * @return
 */
void insertDataLabelCodeWord(CodeWord **head, char *label, enum Are are);

/**
 * @brief Inserts a register code word to the end of the list
 * @param head - the head of the list
 * @param source_register - the source register
 * @param dest_register - the destination register
 * @return
 */
void insertRegisterCodeWord(CodeWord **head, enum Register source_register,
                            enum Register dest_register);

/**
 * @brief Inserts a data number code word to the end of the list
 * @param head - the head of the list
 * @param value - the value to insert
 * @param are - the are of the value
 * @return
 */
void insertDataNumberCodeWord(CodeWord **head, signed int value, enum Are are);

/**
 * @brief Creates a new code word node
 * @param head - the head of the list
 * @return
 */
void printCodeWordList(CodeWord **head);

/**
 * @brief free the code word list
 * @param head - the head of the list
 * @return
 */
void freeCodeWordList(CodeWord **head);

/**
 * @brief Creates a new code word node
 * @param address - the address of the label
 * @param type - the type of the label
 * @return
 */
Label *createLabelNode(const char *name, long address, enum LabelType type);

/**
 * @brief Creates a new code word node
 * @param head - the head of the list
 * @param address - the address of the label
 * @param type - the type of the label
 * @return
 */
void insertLabelNode(Label **head, const char *name, long address, enum LabelType type);

/**
 * @brief free the label list
 * @param head - the head of the list
 * @return
 */
void freeLabelList(Label **head);

/**
 * @brief print the label list
 * @param head - the head of the list
 * @return
 */
void printLabelList(Label **head);

/**
 * @brief find label by name
 * @param head - the head of the list
 * @param name - the name of the label
 * @return Label
 */
Label *findLabelByName(Label **head, const char *name);

/**
 * @brief check if label exists
 * @param head - the head of the list
 * @param name - the name of the label
 * @return int
 */
int labelExists(Label **head, const char *name);

#endif
