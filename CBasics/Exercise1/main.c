#include <stdio.h>
#include <stdbool.h>

int main(void) {
    float busPrice, taxiPrice, money;
    bool walking = false;

    printf("Enter price of the bus ticket: \n");
    scanf("%f", &busPrice);
    printf("Enter price of taxi: \n");
    scanf("%f", &taxiPrice);
    printf("How much money you have: \n");
    scanf("%f", &money);

    while (!walking){
        printf("You have %.2f euros left.\n", money);
        if (money >= taxiPrice || money >= busPrice) {
            int choice;
            printf("Do you want to take \n");
            printf("1) bus (%.2f euros)\n", busPrice);
            printf("2) taxi (%.2f euros)\n", taxiPrice);
            scanf("%d", &choice);
            switch (choice){
                case 1:
                    if (money >= busPrice) {
                        printf("You chose bus.\n");
                        money -= busPrice;
                    }
                    else {
                        printf("You don't have enough money for bus.\n");
                    }
                    break;
                case 2:
                    if (money >= taxiPrice) {
                        printf("You chose taxi.\n");
                       money -= taxiPrice;
                    }
                    else {
                        printf("You don't have enough money for taxi.\n");
                    }
                    break;
                default:
                    printf("Incorrect input!\n");
                    break;
            }
        }else{
            printf("You need to walk. Bye\n");
            walking = true;
        }
    }
}

