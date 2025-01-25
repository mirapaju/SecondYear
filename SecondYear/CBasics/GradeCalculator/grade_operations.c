#include <stdio.h>
#include "grade_calculator.h"

int determine_grade(int subject_score){
    int grade = (subject_score > 100) ? FALSE_VALUE :
                (subject_score >= 90) ? 5 :
                (subject_score >= 80) ? 4 :
                (subject_score >= 70) ? 3 :
                (subject_score >= 60) ? 2 :
                (subject_score >= 50) ? 1 :
                (subject_score >= 0) ? 0 : FALSE_VALUE;
    return grade;
}

//calculate average of all grades, false grade values are not used for counting
double calculate_average(subjects_data *subject, int struct_array_size) {
    int count = 0;
    double sum = 0.0;

    for (int i=0; i<struct_array_size; i++) {
        if (subject[i].grade != FALSE_VALUE) {
            sum += subject[i].grade;
            count++;
        }
    }
    if (count > 0) {
        double average = sum / count;
        if (average < 0 || average > 5) {
            printf("Error counting the average!\n");
            return FALSE_AVERAGE;
        }
        else{
            return average;
        }
    }
    else {
        printf("Error counting the average!\n");
        return FALSE_AVERAGE;
    }
}