#include <stdio.h>
#include <stdlib.h>

typedef struct number
{
    float num;
    struct number *next;
} number;


int main() {
    // store five numbers into linked list
    number *head, *current, *temp;
    head = current = temp = NULL;
    for(int i=0; i<5; i++) {
        temp = (number*)malloc(sizeof(number));
        temp->next = NULL;
        scanf("%f", &(temp->num));
        if(head == NULL) {
            head = current = temp;
        }
        else {
            current->next = temp;
            current = current->next;
        }
    }
    // Calculate average of five numbers
    float ave = 0;
    current = head;
    while(current != NULL) {
        ave += current->num;
        current = current->next;
    }
    ave /= 5;
    printf("%.2f\n", ave);

    // Calculate 1+2+3+...+n
    int n, sum=0;
    scanf("%d", &n);
    for(int i=1; i<=n; i++) {
        sum += i;
    }
    printf("%d", sum);
    return 0;
}