#include <stdio.h>
#include <stdlib.h>

typedef struct value {
    int coin;
    struct value *next;
} value;

int main() {
    // input money
    int money;
    scanf("%d", &money);
    // store value into linked list
    value *head, *current, *temp;
    head = current = temp = NULL;
    for(int i=100; i>=1; i/=10) {
        temp = (value*)malloc(sizeof(value));
        temp->next = NULL;
        temp->coin = money/i;
        money %= i;
        if(head == NULL) {
            head = current = temp;
        }
        else {
            current->next = temp;
            current = current->next;
        }
    }
    // print result
    current = head;
    while(current != NULL) {
        if(current->next == NULL) {
            printf("%d", current->coin);
        }
        else {
            printf("%d ", current->coin);
        }
        current = current->next;
    }
    return 0;
}