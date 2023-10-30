#include <stdio.h>
#include <stdbool.h>

int col, row, arr1[100], arr2[100], arr3[100], queue, count=0, append;
char value[5];

int stackSize();
bool IsFull();
void Push();
bool IsEmpty();
void Pop();
void PopMore();

int main() {
    int func;
    scanf("%d", &queue);
    while(scanf("%d", &func)) {
        if(func == 1) stackSize();
        else if(func == 2) {
            IsFull();
            Push();
        }
        else if(func == 3) {
            IsEmpty();
            Pop();
        }
        else if(func == 4) PopMore();
        else if(func == 5) ;
        else if(func == 6) {
            printf("leave\n");
            break;
        }
    }
    return 0;
}

int stackSize() {
    printf("queue size %d\n", queue);
}

bool IsFull() {
    if(count == queue)
        printf("Stack is full!\n");
}

void Push() {
    if(count < queue) {
        scanf("%d %d %s", &col, &row, value);
        arr1[count] = col;
        arr2[count] = row;
        arr3[count] = value;
        count += 1;
    }
    else if(count == queue) {
        scanf("%d", &append);
        if(append%2 != 0) {
            scanf("%d", &append);
        }
    }
}

bool IsEmpty() {

}

void Pop() {

}

void PopMore() {

}