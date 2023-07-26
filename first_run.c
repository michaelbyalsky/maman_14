#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "globals.h"
#include "helpers.h"
#include "first_run.h"

int process_line(char *line, long *ic, long *dc, DataWord *data_img, Label **labelHead, CodeWord **codeHead);

int is_valid_label(const char *label);

int find_label(char *line, char *label, int *line_index);

int
handle_directive(char *line, int *line_index, long *ic, long *dc, DataWord *data_img, Label **labelHead, char *label);

int is_directive(const char *line, const int *line_index);

int
handle_string_directive(const char *line, int *line_index, long *ic, long *dc, DataWord *data_img, Label **labelHead,
                        char *label);

int handle_data_directive(const char *line, int *line_index, long *ic, long *dc, DataWord *data_img, Label **labelHead,
                          char *label);

int handle_instruction(char *line, int *line_index, long *ic, long *dc, Label **labelHead, CodeWord **codeHead,
                       char *label);

int first_run(char *filename, long *ic, long *dc, DataWord *data_img, Label **labelHead, CodeWord **codeHead) {
    /* read the file */
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: could not open file %s\n", filename);
        return -1;
    }

    char line[MAX_LINE_LENGTH];

    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        process_line(line, ic, dc, data_img, labelHead, codeHead);
    }

}

int process_line(char *line, long *ic, long *dc, DataWord *data_img, Label **labelHead, CodeWord **codeHead) {
    /*line index*/
    int i = 0;

    if (is_empty_line(line)) {
        return 0;
    }
    char label[MAX_LABEL_SIZE];
    label[0] = '\0';
    /* check if line contains a label */
    find_label(line, label, &i);


    SKIP_WHITE_SPACES(line, i);
    if (is_directive(line, &i)) {
        i++;
        handle_directive(line, &i, ic, dc, data_img, labelHead, label);
        return 1;
    }

    if (is_instruction(&line[i])) {
        handle_instruction(line, &i, ic, dc, labelHead, codeHead, label);
        return 1;
    }
}

int handle_instruction(char *line, int *line_index, long *ic, long *dc, Label **labelHead, CodeWord **codeHead,
                       char *label) {
    Instruction instruction = findInstructionByName(line);
    printf("instruction: %s\n", instruction.name);
}

int find_label(char *line, char *label, int *line_index) {
    if (!strchr(line, ':') !=
        NULL) { /* strchr returns a pointer to the first occurence of ":" in the string, if it is not in the string it returns NULL */
        return 0;
    }

    sscanf(line, "%[^:]", label); /* read until ":" the [^:] means read until ":" */
    label[strlen(label)] = '\0'; /* add the null terminator to the label */
    /* move the line pointer to the next character after the label */
    *line_index += strlen(label) + 1;

    /* check if the label is valid */
    if (is_valid_label(label)) {
        return 1;
    }
    return 0;
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

int is_directive(const char *line, const int *line_index) {
    if (line[*line_index] == '.') {
        return 1;
    }
    return 0;
}

int
handle_directive(char *line, int *line_index, long *ic, long *dc, DataWord *data_img, Label **labelHead, char *label) {
    /* extract directive name */
    int directive = find_directive_by_name(&line[*line_index]);
    if (directive == DIRECTIVE_NOT_FOUND) {
        printf("Error: directive not found\n");
        return -1;
    }

    if (directive == DATA) {
        handle_data_directive(line, line_index, ic, dc, data_img, labelHead, label);
    } else if (directive == STRING) {
        handle_string_directive(line, line_index, ic, dc, data_img, labelHead, label);
    } else if (directive == ENTRY) {
        /* handle entry directive */
    } else if (directive == EXTERN) {
        /* handle extern directive */
    }
}

int
handle_string_directive(const char *line, int *line_index, long *ic, long *dc, DataWord *data_img, Label **labelHead,
                        char *label) {
    /* move the line_index 6 characters forward to skip the "string" */
    *line_index += 6;
    SKIP_WHITE_SPACES(line, *line_index);
    /* check if the next character is a double quote */
    if (line[*line_index] != '"') {
        printf("Error: string directive must be followed by a string\n");
        return -1;
    }
    (*line_index)++;

    int temp_index = *line_index;
    /* check there is closing double quote */
    while (line[temp_index] != '"') {
        if (line[temp_index] == '\0') {
            printf("Error: string directive must be followed by a string\n");
            return -1;
        }
        temp_index++;
    }

    insertLabelNode(labelHead, label, *dc, DATA_LABEL);

    /* go until the next double quote and add each character to the data image */
    while (line[*line_index] != '"') {
        data_img[*dc].datatype = STRING;
        data_img[*dc].string[0] = line[*line_index];
        data_img[*dc].string[1] = '\0';
        (*dc)++;
        (*line_index)++;
    }
    (*line_index)++;
}

int handle_data_directive(const char *line, int *line_index, long *ic, long *dc, DataWord *data_img, Label **labelHead,
                          char *label) {
    /* move the line_index 4 characters forward to skip the "data" */
    *line_index += 5;
    char line_copy[MAX_LINE_LENGTH];
    line_copy[0] = '\0';
    strcpy(line_copy, &line[*line_index]);
    remove_white_spaces(line_copy, 0);
    int temp_index = 0;
    int start_dc = *dc;
    /* while is a character */
    while (line_copy[temp_index] != '\000' && line_copy[temp_index] != "\0" && line_copy[temp_index] != "\n") {
        char temp_num[MAX_LINE_LENGTH] = {'\0'};
        if (line_copy[temp_index] == ',') {
            temp_index++;
            continue;
        }
        /* if - */
        if (line_copy[temp_index] == '-') {
            temp_num[0] = '-';
            temp_index++;
        }
        /* if number */
        if (isdigit(line_copy[temp_index])) {
            while (isdigit(line_copy[temp_index])) {
                temp_num[strlen(temp_num)] = line_copy[temp_index];
                temp_index++;
            }
            data_img[*dc].datatype = DATA;
            data_img[*dc].number = atoi(temp_num);
            (*dc)++;
            continue;
        }
        temp_index++;
    }
    if (*dc - start_dc != 0) {
        insertLabelNode(labelHead, label, start_dc, DATA_LABEL);
    }
}
