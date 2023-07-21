#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "globals.h"
#include "helpers.h"
#include "first_run.h"

int process_line(char *line, long *ic, long *dc, unsigned long *data_img);

int is_valid_label(const char *label);

int find_label(char *line, char *label, int *line_index);

int handle_directive(char *line, int *line_index, long *ic, long *dc, unsigned long *data_img);

int is_directive(char *line, int *line_index);

int handle_string_directive(const char *line, int *line_index, long *ic, long *dc, unsigned long *data_img);

int first_run(char *filename, long *ic, long *dc, unsigned long *data_img) {
    /* read the file */
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: could not open file %s\n", filename);
        return -1;
    }

    char line[MAX_LINE_LENGTH];

    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        process_line(line, ic, dc, data_img);
    }

}

int process_line(char *line, long *ic, long *dc, unsigned long *data_img) {
    /*line index*/
    int i = 0;
    /* check if line is empty */
    if (is_empty_line(line)) {
        return 0;
    }
    char label[MAX_LABEL_SIZE];
    int is_label;
    /* check if line contains a label */
    is_label = find_label(line, label, &i);


    SKIP_WHITE_SPACES(line, i);
    if (is_directive(line, &i)) {
        i++;
        handle_directive(line, &i, ic, dc, data_img);
    }
}

int find_label(char *line, char *label, int *line_index) {
    int i = 0; /* Initialize the index variable */
    SKIP_WHITE_SPACES(line, i);
    line = line + i;
    *line_index += i;

    if (!strchr(line, ':') !=
        NULL) { /* strchr returns a pointer to the first occurence of ":" in the string, if it is not in the string it returns NULL */
        return 0;
    }

    sscanf(line, "%[^:]", label); /* read until ":" the [^:] means read until ":" */
    /* move the line pointer to the next character after the label */
    int label_len = strlen(label);
    *line_index += label_len + 1;

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

int is_directive(char *line, int *line_index) {
    if (line[*line_index] == '.') {
        return 1;
    }
    return 0;
}

int handle_directive(char *line, int *line_index, long *ic, long *dc, unsigned long *data_img) {
    /* extract directive name */
    int directive = find_directive_by_name(&line[*line_index]);
    if (directive == DIRECTIVE_NOT_FOUND) {
        printf("Error: directive not found\n");
        return -1;
    }

    if (directive == DATA) {
        /* handle data directive */
    } else if (directive == STRING) {
        handle_string_directive(line, line_index, ic, dc, data_img);
    } else if (directive == ENTRY) {
        /* handle entry directive */
    } else if (directive == EXTERN) {
        /* handle extern directive */
    }
}

int handle_string_directive(const char *line, int *line_index, long *ic, long *dc, unsigned long *data_img) {
    /* move the line_index 6 characters forward to skip the "string" */
    *line_index += 6;
    SKIP_WHITE_SPACES(line, *line_index);
    /* check if the next character is a double quote */
    if (line[*line_index] != '"') {
        /* check if the next character is -30 -128 -100 */
        if (line[*line_index] == -30 && line[*line_index + 1] == -128 && line[*line_index + 2] == -100) {
            (*line_index) += 2;
        } else {
            printf("Error: string directive must be followed by a string\n");
            return -1;
        }
    }
    (*line_index)++;

    int temp_index = *line_index;
    /* check there is closing double quote */
    while (line[temp_index] != '"' && (line[temp_index] != -30 && line[temp_index + 1] != -128 && line[temp_index + 2] != -99)) {
        if (line[temp_index] == '\0') {
            printf("Error: string directive must be followed by a string\n");
            return -1;
        }
        temp_index++;
    }

    /* go until the next double quote and add each character to the data image */
    while (line[*line_index] != '"' && (line[*line_index] != -30 && line[*line_index + 1] != -128 && line[*line_index + 2] != -99)) {
        data_img[*dc] = line[*line_index];
        (*dc)++;
        (*line_index)++;
    }
    (*line_index)++;
}
