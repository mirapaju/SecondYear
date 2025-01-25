#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct node {
    int number;
    struct node *next;
} nnode;

void print_list(nnode* head);
void free_alloc(nnode* head);

int main() {
    bool run_program =true;
    char input[80];
    int num = 0;
    nnode *head = NULL;
    nnode *new_head;

    while (run_program) {
        printf("Enter a number or type \"end\" to stop:\n");
        if (fgets(input, sizeof(input), stdin) != NULL){
            if (strcmp(input, "end\n") == 0) {
                print_list(head);
                run_program = false;
            }
            else if (sscanf(input, "%d", &num) == 1) {
                new_head = (nnode *) malloc((sizeof(nnode)));          //varatun muistin alkuun toinen pointer
                if (new_head != NULL) {                                     //jos allocate onnistuu
                    new_head->number = num;                                 //nykyisen ekan noden number numiksi
                    new_head->next = head;                                  //nykyisen ekan noden next-pointer osottamaan headiin(lisätty jäsen kiinnittyy seuraavaan)
                    head = new_head;                                        //siirretään head osottamaan taas alkuun, nyt head ja new_head taas alussa
                } else {
                    printf("Error allocating memory!\n");
                    return 1;
                }
            }
            else {
                printf("Invalid input!\n");
            }
        }
        else {
            printf("Error reading input!\n");
        }
    }
    free_alloc(head);
    return 0;
}

void print_list(nnode* head) {
    printf("Numbers entered:");
    nnode* current = head;
    while (current != NULL) {
        if (current->next == NULL){
            printf(" %d.", current->number);
            current = current->next;
        }
        else {
            printf(" %d, ", current->number);
            current = current->next;
        }
    }

}

void free_alloc(nnode* head) {
    nnode* current = head;                  //nykynen alkuun
    while (current != NULL) {
        nnode* next_node = current->next;  //seuraava nykysestä seuraava
        free(current);             //vapauta nykynen
        current = next_node;               //siirry nykysestä seuraavaan
    }
}