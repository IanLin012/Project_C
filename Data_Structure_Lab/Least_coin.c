#include <stdio.h>
#include <stdlib.h>

typedef struct value {
    int coin;
    struct value *next;
} value;

int main() {
    int money;
    scanf("%d", &money);
    value *head, *current=NULL;
    for(int i=100; i>=1; i/=10) {
        head = (value*)malloc(sizeof(value));
        head->coin = money/i;
        money /= i;
        head->next = current;
        current = head;
    }
    value *temp = head;
    while(head != NULL) {
        printf("%d ", temp->coin);
        temp = temp->next;
    }       
    return 0;
}