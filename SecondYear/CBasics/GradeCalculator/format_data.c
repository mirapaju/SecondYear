#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include "grade_calculator.h"
#include "format_strings.h"

//extracts values from structure array and sends to formatting
void construct_table(char *buffer, size_t *buffer_size, char *name, subjects_data *subject, double average, int struct_array_size) {
    format_lines(&buffer, buffer_size,
                 "\n"TABLE_SEPARATOR_LINE
                 STUDENT_NAME_FORMAT
                 TABLE_SEPARATOR_LINE
                 TABLE_HEADER_FORMAT
                 TABLE_SEPARATOR_LINE,
                 name, "Subject", "Score", "Grade");

    for (int i = 0; i < struct_array_size; i++) {
        if (subject[i].name[0] == '\0') strcpy(subject[i].name, "N/A");
        format_lines(&buffer, buffer_size, SUBJECT_NAME_FORMAT, subject[i].name);

        if(subject[i].score == FALSE_VALUE || subject[i].score == INIT_VALUE) {
            format_lines(&buffer, buffer_size, SCORE_STR_FORMAT, "N/A");
            printf("Error handling the %s score!\n", subject[i].name);
        }
        else{
            format_lines(&buffer, buffer_size, SCORE_INT_FORMAT, subject[i].score);
        }

        if (subject[i].grade == FALSE_VALUE || subject[i].grade == INIT_VALUE) {
            printf("Error handling the %s grade!\n", subject[i].name);
            format_lines(&buffer, buffer_size, GRADE_STR_FORMAT, "N/A");
        }
        else{
            if (subject[i].grade == 0) format_lines(&buffer, buffer_size,GRADE_STR_FORMAT, "FAIL");
            else format_lines(&buffer, buffer_size, GRADE_INT_FORMAT, subject[i].grade);
        }
    }
    format_lines(&buffer, buffer_size, "\n");
    (average == FALSE_AVERAGE) ?
    format_lines(&buffer, buffer_size, AVERAGE_STR_FORMAT, "N/A") : format_lines(&buffer, buffer_size, AVERAGE_INT_FORMAT, average);

    format_lines(&buffer, buffer_size, TABLE_SEPARATOR_LINE);
}


//formats the given data and stores to buffer
void format_lines(char **buffer, size_t *buffer_size, const char *format_string, ...) {
    size_t current_position = strlen(*buffer);
    va_list args;
    va_start(args, format_string);
    int result = vsnprintf(*buffer + current_position, *buffer_size - current_position, format_string, args);
    va_end(args);

    //in case of buffer overflow, reallocates memory
    if (result < 0 || (size_t)result >= (*buffer_size - current_position)) {
        *buffer_size *= 2;
        char *new_buffer = realloc(*buffer, *buffer_size);
        if (new_buffer == NULL) {
            fprintf(stderr, "Memory reallocation failed, unable to print or write data!\n");
            free(*buffer);
            exit(1);
        }
        *buffer = new_buffer;

        //tries to format again
        va_start(args, format_string);
        result = vsnprintf(*buffer + current_position, *buffer_size - current_position, format_string, args);
        va_end(args);

        //in case the vsnprintf() fails for something else than buffer issues
        if (result < 0) {
            fprintf(stderr, "Error encountered during formatting after reallocation!\n");
        }
    }
    if (result < 0) {
        fprintf(stderr, "Error encountered during formatting!\n");
    }
}

