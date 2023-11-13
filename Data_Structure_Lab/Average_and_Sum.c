#include <stdio.h>
#include <stdlib.h>

typedef struct number
{
    float num;
    struct number *next;
} number;


int main() {
    // Calculate ave of five numbers
    number *head=NULL, *current;
    float ave = 0;
    for(int i=0; i<5; i++) {
        current = (number*)malloc((sizeof(number)));
        scanf("%f", &(current->num));
        ave += (current->num);
        current->next = head;
        head = current;
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