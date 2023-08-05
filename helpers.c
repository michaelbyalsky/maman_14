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

        if (!isdigit(*str)) {
            return 0;
        }
        str++;
    }
    return 1;
}


/**
 * @brief gets a string that it's first character is a number and returns the number
 * @param string
 * @return FuncResult
 */
FuncResult get_number_from_string(char *string, int *number) {
    FuncResult func_result;
    char error_message[MAX_LINE_LENGTH];
    int i = 0;
    char temp_num[MAX_LINE_LENGTH];
    error_message[0] = '\0';
    temp_num[0] = '\0';
    if (string[i] == '-') {
        temp_num[0] = '-';
        i++;
    }


    if (isdigit(string[i])) {
        while (string[i] != '\0' && string[i] != ' ' && string[i] != '\t' && string[i] != '\n' &&
               string[i] != 13) {
            temp_num[i] = string[i];
            i++;
        }
        temp_num[i] = '\0';

        /* check if the number found */

        if (!is_valid_number(temp_num)) {
            strcpy(func_result.message, "Error: invalid number\n");
            func_result.result = 0;
            return func_result;
        }

        *number = atoi(temp_num);
        func_result.result = 1;
        return func_result;
    }

    func_result.result = 0;
    return func_result;
}


FuncResult is_valid_label(const char *label) {
    FuncResult func_result;
    char error_message[MAX_LINE_LENGTH];
    int i;
    size_t len = strlen(label);
    error_message[0] = '\0';
    /* if last char is empty space ot "\0" remove it */
    if (label[len - 1] == 13) {
        len--;
    }

    /* Check the length of the label (must be at most 31 characters) */
    if (len == 0 || len > 31) {
        strcpy((char *) func_result.message, "Error: invalid label- label must be at most 31 characters\n");
        func_result.result = 0;
        return func_result;
    }

    /* Check if the label starts with an alphabetic character */
    if (!isalpha(label[0])) {
        strcpy((char *) func_result.message, "Error: invalid label- label must start with an alphabetic character\n");
        func_result.result = 0;
        return func_result;
    }

    /* Check if the remaining characters are valid label characters */
    for (i = 1; i < len; ++i) {
        /* Check if the character is an alphanumeric character */
        if (!isalnum(label[i])) {
            strcpy((char *) func_result.message, "Error: invalid label- label must contain only alphanumeric characters\n");
            func_result.result = 0;
            return func_result;
        }
    }

    func_result.result = 1;
    return func_result;
}


/**
 * @brief gets a string and returns the label in it
 * @param string
 * @param label
 * @return int
 */
int get_label_from_string(const char *string, char *label) {
    int i = 0;
    if (!isalpha(string[0])) {
        return 0;
    }
    while (string[i] != '\0' && string[i] != ' ' && string[i] != '\t' && string[i] != ':' && string[i] != '\n') {
        label[i] = string[i];
        i++;
    }
    label[i] = '\0';
    remove_white_spaces(label, 0);
    return 1;
    /*is_valid_label(label);*/
}


void logger_error(const char *message, int line_number) {
    printf("Error: %s in line %d\n", message, line_number);
}

void logger_warning(const char *message, int line_number) {
    printf("Warning: %s in line %d\n", message, line_number);
}

int is_line_80_chars_long(const char *line) {
    return strlen(line) <= 80;
}