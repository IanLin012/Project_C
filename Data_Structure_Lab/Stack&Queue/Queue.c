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
    int size;
    int max_size;
    int top;
    int back;
    Item *queue;
} Queue;

int stackSize(Queue);
void appendSize(Queue *, int);
bool isFull(Queue);
void push(Queue *);
bool isEmpty(Queue);
Item pop(Queue *);
Item popMore(Queue *, int);

int main() {
    Queue s = {0, 0, 0, 0, NULL};
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
                printf("No item, queue is empty!\n");
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

int stackSize(Queue s) {
    printf("queue size %d\n", s.max_size);
    return s.max_size;
}

void appendSize(Queue *s, int append) {
    s->queue = (Item *) realloc(s->queue, sizeof(Item) * (s->max_size + append));
    s->max_size += append;
}

bool isFull(Queue s) {
    return s.size >= s.max_size;
}

void push(Queue *s) {
    if(!isFull(*s)) {
        int a, b; char *str;
        str = (char*) malloc(sizeof(char) * 5);
        Item item;
        scanf("%d %d %s", &a, &b, str);
        item.raw = a, item.column = b;
        item.value = str;
        s->queue[s->back] = item;
        s->back++, s->size++;
        if(s->top >= s->max_size)
            s->top -= s->max_size;
    }
    else {
        printf("Queue is full!\n");
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

bool isEmpty(Queue s) {
    return s.size == 0;
}

Item pop(Queue *s) {
    Item item;
    if(!isEmpty(*s)) {
        item = s->queue[s->top];
        s->top++, s->size--;
        if(s->top >= s->max_size)
            s->top -= s->max_size;
    }
    return item;
}

Item popMore(Queue *s, int n) {
    for(int i=0; i<n; i++) {
        if(!isEmpty(*s))
            pop(s);
        else
            break;
    }
}