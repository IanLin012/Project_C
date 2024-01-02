#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define MAX_ELEMENTS 100+1

typedef struct Element{
    int priority;
    char what[MAX_ELEMENTS];
    char who[MAX_ELEMENTS];
    char when[MAX_ELEMENTS];
    char where[MAX_ELEMENTS];
} Element;

Element buffer[MAX_ELEMENTS], heap[MAX_ELEMENTS];
bool contains[MAX_ELEMENTS] = {0};
int keyRecords[MAX_ELEMENTS];
int bfSize=0, hpSize=0;

Element inputElement() {
    Element e;
    scanf("%d%*c", &e.priority);
    fgets(e.who, MAX_ELEMENTS, stdin);
    e.who[strcspn(e.who, "\n")]=0;
    fgets(e.what, MAX_ELEMENTS, stdin);
    e.what[strcspn(e.what, "\n")]=0;
    fgets(e.when, MAX_ELEMENTS, stdin);
    e.when[strcspn(e.when, "\n")]=0;
    fgets(e.where, MAX_ELEMENTS, stdin);
    e.where[strcspn(e.where, "\n")]=0;

    return e;
}

void inputBuffer() {
    if (bfSize >= MAX_ELEMENTS) {
        printf("Buffer full\n");
        return;
    }

    Element e = inputElement();

    while (contains[e.priority]) {
        printf("KEY EXIST: ");
        for (int i=0; i<bfSize; i++) {
            printf("%d%s", keyRecords[i], (i==bfSize-1) ? "\n" : " ");
        }
        printf("New key:\n");
        scanf("%d%*c", &e.priority);
    }
    
    contains[e.priority] = true;
    keyRecords[bfSize] = e.priority;
    buffer[bfSize] = e;
    bfSize++;
    
    printf("OK\n");
}

void printData(Element e) {
    printf("%d\n%s, %s, %s, %s\n", 
            e.priority, 
            e.who, e.what, e.when, e.where
    );
}

void printBufferData(int size) {
    if (!size) {
        printf("No Data\n");
        return;
    }
    for (int i=0; i<size; i++) {
        printData(buffer[i]);
    }
}

int toPIndex(int i) {
    return i/2 - !(i%2);
}

int toLIndex(int i) {
    return i*2 + 1;
}

int toRIndex(int i) {
    return i*2 + 2;
}

void pushHeap(Element e) {
    if (hpSize >= MAX_ELEMENTS) {
        printf("Heap full\n");
        return;
    }
    int i = hpSize++;
    while ((toPIndex(i)>=0) && (e.priority > heap[toPIndex(i)].priority)) {
        heap[i] = heap[toPIndex(i)];
        i = toPIndex(i);
    }
    heap[i] = e;

}


Element popHeap(Element *heap, int *size) {
    Element top = {-1};
    if (!*size) {
        printf("No Data\n");
        return top;
    }
    top = heap[0];
    Element e = heap[--*size];
    int i=1;
    while (i < *size) {
        if ((i < *size) && (heap[i].priority < heap[i+1].priority))
            i++;
        if (e.priority >= heap[i].priority)
            break;
        heap[toPIndex(i)] = heap[i];
        i = toLIndex(i);
    }
    heap[toPIndex(i)] = e;
    return top;
}

void printHeapData(int size) {
    if (!size) {
        printf("No Data\n");
        return;
    }
    Element tmp[MAX_ELEMENTS], sorted[MAX_ELEMENTS];
    char output[MAX_ELEMENTS * size];

    for (int i=0; i<size; i++) {
        tmp[i] = heap[i];
    }
    for (int i=0, s=size; s; i++) {
        sorted[i] = popHeap(tmp, &s);
    }
    for (int i=0; i<size; i++) {
        printf("%d%s", sorted[i].priority, (i==size-1) ? "\n" : " ");
    }
    for (int i=0; i<size; i++) {
        printData(sorted[i]);
    }
}

void makeHeap() {
    if (bfSize) {
        hpSize = 0;
        memset(contains, 0, sizeof(contains));
    }
    else {
        printHeapData(0); 
        return;
    }

    for (int i=0; i<bfSize; i++) {
        pushHeap(buffer[i]);
    }
    bfSize = 0;
    printHeapData(hpSize);
}

void inputHeap() {
    Element e = inputElement();
    pushHeap(e);
    printHeapData(hpSize);
}

int main() {
    char *str = (char*) malloc( sizeof(char) * MAX_ELEMENTS );
    while(fgets(str, MAX_ELEMENTS, stdin) != NULL) {
        switch (str[0]) {
        case '1':
            inputBuffer();
            break;

        case '2':
            printBufferData(bfSize);
            break;

        case '3':
            makeHeap();
            break;

        case '4':
            printHeapData(hpSize);
            break;

        case '5': ;
            Element e = popHeap(heap, &hpSize);
            if (e.priority != -1) 
                printData(e);
            break;

        case '6': ;
            inputHeap();
            break;

        case '0':
            printf("quit\n");
            return 0;

        default:
            printf("ERROR\n");
            break;
        }
    }
    return 0;
}