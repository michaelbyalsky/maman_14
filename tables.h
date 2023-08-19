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
 * @param totalWords - the total words of the instruction
 * @param ic - the ic of the instruction
 * @return
 */
void
insert_instruction_code_word(CodeWord **head, enum AddressMethod source, unsigned int opcode, enum AddressMethod dest,
                             int totalWords, unsigned int ic, int unsigned address);


/**
 * @brief Inserts a data label code word to the end of the list
 * @param head - the head of the list
 * @param label - the label to insert
 * @param are - the are of the label
 * @return
 */
void insert_data_label_code_word(CodeWord **head, char *label, enum Are are, int unsigned address);

/**
 * @brief Inserts a register code word to the end of the list
 * @param head - the head of the list
 * @param source_register - the source register
 * @param dest_register - the destination register
 * @return
 */
void insert_register_code_word(CodeWord **head, enum Register source_register,
                               enum Register, int unsigned address);

/**
 * @brief Inserts a data number code word to the end of the list
 * @param head - the head of the list
 * @param value - the value to insert
 * @param are - the are of the value
 * @return
 */
void insert_data_number_code_word(CodeWord **head, signed int value, enum Are are, int unsigned address);

/**
 * @brief free the code word list
 * @param head - the head of the list
 * @return
 */
void free_code_word_list(CodeWord **head);

/**
 * @brief Creates a new code word node
 * @param address - the address of the label
 * @param type - the type of the label
 * @return
 */
Label *create_label_node(const char *name, unsigned int address, enum LabelType type);

/**
 * @brief Creates a new code word node
 * @param head - the head of the list
 * @param address - the address of the label
 * @param type - the type of the label
 * @return
 */
void insert_label_node(Label **head, const char *name, unsigned int address, enum LabelType type);

/**
 * @brief free the label list
 * @param head - the head of the list
 * @return
 */
void free_label_list(Label **head);

/**
 * @brief find label by name
 * @param head - the head of the list
 * @param name - the name of the label
 * @return Label
 */
Label *find_label_by_name(Label **head, const char *name);

/**
 * @brief check if label exists
 * @param head - the head of the list
 * @param name - the name of the label
 * @return int
 */
int label_exists(Label **head, const char *name);


/**
 * @brief update the label type
 * @param head - the head of the list
 * @param name - the name of the label
 * @param type - the type of the label
 * @return int
 */
int update_label_type(Label **head, const char *name, enum LabelType type);

/**
 * @brief find code word by ic
 * @param head
 * @param ic
 * @return
 */
CodeWord *find_code_word_by_ic(CodeWord **head, unsigned int ic);


/**
 * @brief Creates a new data word node
 * @param dataType - the type of the data
 * @return
 */
DataWord *create_data_word_node(enum Directives dataType);

/**
 * @brief Inserts a data number code word to the end of the list
 * @param head
 * @param number
 */
void insert_number_data_word(DataWord **head, int number, int unsigned address);

/**
 * @brief Inserts a data string code word to the end of the list
 * @param head
 * @param string
 */
void insert_string_data_word(DataWord **head, char *string, int unsigned address);

/**
 * @brief free the data word list
 * @param head
 */
void free_data_word_list(DataWord **head);

#endif
