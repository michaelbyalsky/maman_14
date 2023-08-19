#ifndef MAMAN_14_HELPERS_H
#define MAMAN_14_HELPERS_H

#include "globals.h"

#define SKIP_WHITE_SPACES(string, index) \
    while ((string)[(index)] && ((string)[(index)] == '\t' || (string)[(index)] == ' ')) { \
        ++(index); \
    }

/**
 * @brief the function get a number as string and returns the number in int type
 * @param string
 * @param number
 * @return FuncResult
 */
FuncResult get_number_from_string(char *string, int *number);

/**
 * @brief the function checks if the string is a valid label
 * @param label
 * @return FuncResult
 */
FuncResult is_valid_label(const char *label);

/**
 * @brief the function gets a string and returns the label in the string
 * @param string
 * @param label
 * @return int
 */
int get_label_from_string(const char *string, char *label);

/**
 * @brief checks if the line is empty
 * @param s
 * @return int
 */
int is_empty_line(char *s);

/**
 * @brief the function removes white spaces from string
 * @param str
 * @param start_index
 */
void remove_white_spaces(char *str, int start_index);

/**
 * @brief the function checks if the number is valid
 * @param str
 * @return int
 */
int is_valid_number(const char *str);

/**
 * @brief logger error
 * @param message - the message to print
 * @param line_number - the line number in the file
 */
void logger_error(const char *message, int line_number);

/**
 * @brief logger error
 * @param message - the message to print
 * @param line_number - the line number in the file
 */
void logger_warning(const char *message, int line_number);

/**
 * @brief Check if line not more that 80 chars long
 * @param line
 * @return int
 */
int is_line_80_chars_long(const char *line);

/**
 * @brief Check if line not more that 80 chars long
 * @param num
 * @return int
 */
int is10BitsSigned(int num);


/**
 * @brief Check if line not more that 80 chars long
 * @param num
 */
int is12BitsSigned(int num);


/**
 * @brief create new file name with suffix
 * @param filename
 * @param suffix
 * @return
 */
char *getNewFileName(const char *filename, const char *suffix);

char *remove_new_line_char_from_string(char *string);

/**
 * @brief the function checks if the file ends with .as
 * @param filename
 * @return
 */
int validate_file_ends_with_as(const char *filename);

#endif
