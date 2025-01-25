#include <stdio.h>
#include <stdbool.h>

void printInfo(int gradeArray[], int studentCount);

int main() {
    int studentCount, grade, studentNumber;
    int stopProgram = 0;
    int backToStudentChoice = -1;
    bool runProgram = true;
    bool insertGrade = true;

    printf("How many students?\n");
    scanf("%d", &studentCount);

    int gradeArray[studentCount];

    for (int i =0; i<studentCount; i++){
        gradeArray[i] = -1;
    }

    while (runProgram) {
        printf("Enter student number (1 - %d) or 0 to stop:\n", studentCount);
        scanf("%d", &studentNumber);
        if ((studentNumber > 0) && (studentNumber <= studentCount)){
            while (insertGrade) {
                printf("Enter grade (0 - 5) for student %d or -1 to cancel:\n", studentNumber);
                scanf("%d", &grade);
                if ((grade > 0) && (grade <= 5)) {
                    gradeArray[studentNumber - 1] = grade;
                    insertGrade = false;
                }
                else if (grade == backToStudentChoice){
                    insertGrade = false;
                }
                else{
                    printf("Invalid grade!\n");
                }
            }
        }
        else if (studentNumber == stopProgram){
            printInfo(gradeArray, studentCount);
            runProgram = false;
        }
        else {
            printf("Invalid student number!\n");
        }
    }
    return 0;
}

void printInfo(int gradeArray[], int studentCount) {
    printf("%-12s%-12s\n", "Student", "Grade");
    for (int i = 0; i < studentCount; i++) {
        printf("%-12d", i + 1);
        if (gradeArray[i] == -1) {
            printf("N/A\n");
        } else {
            printf("%-12d\n", gradeArray[i]);
        }
    }
}