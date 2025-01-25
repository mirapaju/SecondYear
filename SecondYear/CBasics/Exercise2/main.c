#include <stdio.h>

#define MONTHS 12

int main() {
    float income[MONTHS];
    float taxAmount[MONTHS];
    float incomeLimit, taxRate, higherTaxRate;
    float incomeSum = 0;

    printf("Enter tax rate: \n");
    scanf("%f", &taxRate);
    printf("Enter income limit: \n");
    scanf("%f", &incomeLimit);
    printf("Enter tax rate for income over the limit: \n");
    scanf("%f", &higherTaxRate);

    for (int i=0; i<MONTHS; i++ ){
        printf("Enter income for month %i: \n",i+1);
        scanf("%f",&income[i]);
        incomeSum += income[i];
        if (incomeSum > incomeLimit){
            if ((incomeSum - income[i]) < incomeLimit){
                float higherTaxIncome = incomeSum - incomeLimit;
                float lowerTaxIncome = income[i] - higherTaxIncome;
                taxAmount[i] = (lowerTaxIncome * (taxRate / 100)) + (higherTaxIncome * (higherTaxRate / 100));
            }
            else {
                taxAmount[i] = income[i] * (higherTaxRate /100);
            }
        }
        else {
            taxAmount[i] = income[i] * (taxRate / 100);
        } //printf("income: %f, tax: %f, incomesum: %f\n", income[i], taxAmount[i], incomeSum);
    }
    printf("%5s%11s%10s\n", "month", "income", "tax");
    for (int i=0; i<MONTHS; i++){
        printf("%5d",i+1);
        printf("%11.2f",income[i]);
        printf("%10.2f\n",taxAmount[i]);
    }
    return 0;
}
