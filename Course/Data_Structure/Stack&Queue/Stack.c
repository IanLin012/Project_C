#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#define INIT_SIZE 10

typedef struct {
    int raw;
    int column;
    char *value;
} Item;

typedef struct {
    int max_size;
    int top;
    Item *stack;
} Stack;

int stackSize(Stack);
void appendSize(Stack *, int);
bool isFull(Stack);
void push(Stack *);
bool isEmpty(Stack);
Item pop(Stack *);
Item popMore(Stack *, int);


int main() {
    Stack s = {0,0,NULL};
    int foo;
    scanf("%d", &foo);
    appendSize(&s, foo);
    while (scanf("%d%*c", &foo)) {
        switch (foo) {
        case 1:
            stackSize(s);
            break;
        case 2:
            push(&s);
            break;
        case 3:
            if(!isEmpty(s)) {
                Item i = pop(&s);
                printf("Item %d %d %s poped\n", i.raw, i.column, i.value);
            }
            else
                printf("No item, stack is empty!\n");
            break;
        case 4:
            scanf("%d", &foo);
            popMore(&s, foo);
            break;
        case 5:
            printf("%s\n", isFull(s) ? "Full!" : "Not full!");
            break;
        case 6:
            printf("leave\n");
            return 0;
        default:
            continue;
        }
    }
}

int stackSize(Stack s) {
    printf("stack size %d\n", s.max_size);
    return s.max_size;
}

void appendSize(Stack *s, int append) {
    s->stack = (Item *) realloc(s->stack, sizeof(Item) * (s->max_size + append));
    s->max_size += append;
}

bool isFull(Stack s) {
    return s.top >= s.max_size;
}

void push(Stack *s) {
    if(!isFull(*s)) {
        int a, b; char *str;
        str = (char*) malloc(sizeof(char) * 5);
        Item item;
        scanf("%d %d %s", &a, &b, str);
        item.raw = a, item.column = b;
        item.value = str;
        s->stack[s->top++] = item;
    } else {
        printf("Stack is full!\n");
        int size;
        while(true) {
            scanf("%d", &size);
            if(size%2)
                printf("Please enter a multiple of 2!\n");
            else {
                appendSize(s, size);
                break;
            }
        }
        return;
    }
}

bool isEmpty(Stack s) {
    return s.top == 0;
}

Item pop(Stack *s) {
    if(!isEmpty(*s))
        return s->stack[--s->top];
}

Item popMore(Stack *s, int n) {
    for(int i=0; i<n; i++) {
        if(!isEmpty(*s))
            pop(s);
        else
            break;
    }
}