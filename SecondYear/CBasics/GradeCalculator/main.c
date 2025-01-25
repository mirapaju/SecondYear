#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grade_calculator.h"

//calculates grades and the average of grades based on user inputs
//prints the data into console and writes to file
int main() {
    char name_prompt[PROMPT_SIZE], score_prompt[PROMPT_SIZE];
    char student_name[NAME_SIZE], subject_name[NAME_SIZE];
    int struct_array_size, subject_score;
    int max_score = 100;
    int min_limit_subjects =-1;
    int min_limit_score =0;
    double average;
    FILE *file;

    printf("Welcome to the Student Grade Calculator!\n");
    read_string(student_name, "Please enter your name:");
    int NO_UPPERLIMIT = -1;
    struct_array_size = read_int("How many subjects do you want to calculate grades for:", min_limit_subjects, NO_UPPERLIMIT);

    subjects_data *subject = (subjects_data *)malloc(sizeof(subjects_data) * (struct_array_size));

    for (int i = 0; i < struct_array_size; i++) {
        memset(subject[i].name, 0, sizeof(subject[i].name));
        subject[i].score = INIT_VALUE;
        subject[i].grade = INIT_VALUE;
    }

    if (subject != NULL) {
        for (int i = 0; i < struct_array_size; i++) {
            sprintf(name_prompt, "\nEnter subject %d name:", i + 1);
            read_string(subject_name, name_prompt);
            strcpy(subject[i].name, subject_name);
            sprintf(score_prompt, "Enter your score for %s (0-100):", subject_name);
            subject_score = read_int(score_prompt, min_limit_score, max_score);

            if (subject_score < 0 || subject_score > 100) {
                printf("Error in handling %s score! Not able to determine the grade, therefore value is not used when counting the average.\n", subject_name);
                subject[i].score = FALSE_VALUE;
                subject[i].grade = FALSE_VALUE;
            }
            subject[i].score = subject_score;
            subject[i].grade =  determine_grade(subject_score);
           }
        average = calculate_average(subject, struct_array_size);
    }
    else {
        fprintf(stderr, "Error allocating memory!\n");
        return 1;
    }
    int table_line_count = 7;
    int min_buff_size = table_line_count*TABLE_LINE_SIZE;
    size_t buffer_size = min_buff_size + struct_array_size * TABLE_LINE_SIZE;
    char *buffer = (char *)malloc(buffer_size * sizeof(char));
    if (buffer != NULL) {
        memset(buffer, 0, sizeof(*buffer));
        construct_table(buffer, &buffer_size, student_name, subject, average, struct_array_size);
        if (printf("%s", buffer) < 0) {
            printf("Error printing to console!\n");
        }
        file = fopen("grades.txt", "w");
        if (file == NULL) {
            printf("Error opening the file!");
            return 1;
        }
        else{
            if (fprintf(file, "%s", buffer) < 0) {
                printf("Error writing to file!");
            }
        }
        fclose(file);
        free(buffer);
    }
    else{
        fprintf(stderr, "Error allocating memory!\n");
        return 1;
    }
    free(subject);
    return 0;
}
