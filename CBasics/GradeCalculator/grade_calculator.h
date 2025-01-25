#include <stdbool.h>
#ifndef PROJECT_FUNCTIONS_H
#define PROJECT_FUNCTIONS_H

#define NAME_SIZE 25
#define PROMPT_SIZE 100
#define FALSE_VALUE (-1)
#define FALSE_AVERAGE (-1.00)
#define TABLE_LINE_SIZE 43
#define INIT_VALUE (-2)

typedef struct {
    char name[NAME_SIZE];
    int score;
    int grade;
} subjects_data;

void read_string(char *name, const char *prompt);
int read_int(const char *prompt, int min_limit, int max_limit);
int determine_grade(int subject_score);
double calculate_average(subjects_data *subject, int struct_array_size);
void construct_table(char *buffer, size_t *buffer_size, char *name, subjects_data *subject, double average, int struct_array_size);
void format_lines(char **buffer, size_t *buffer_size, const char *format_string, ...);

#endif //PROJECT_FUNCTIONS_H
