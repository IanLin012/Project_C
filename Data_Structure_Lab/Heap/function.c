#include <stdio.h>
#include <string.h>

#define maxSize 100

struct element {
    int index;
    int key[maxSize];
    char c1[maxSize][maxSize], c2[maxSize][maxSize], c3[maxSize][maxSize], c4[maxSize][maxSize];
} element;


void checkKey(int key) {
    for(int i=0; i<element.index; i++) {
        if(element.key[i] == key) {
            printf("KEY EXIST:");
            for(int j=0; j<element.index; j++) {
                printf(" %d", element.key[j]);
            }
            printf("\nNew key:\n");
            scanf("%d", &key);
            element.key[element.index] = key;
            break;
        }
    }
}

void input() {
    scanf("%d", &element.key[element.index]);
    getchar();
    char temp1[maxSize], temp2[maxSize], temp3[maxSize], temp4[maxSize];
    fgets(temp1, sizeof(temp1), stdin);
    temp1[strlen(temp1)-1] = '\0';
    fgets(temp2, sizeof(temp2), stdin);
    temp2[strlen(temp2)-1] = '\0';
    fgets(temp3, sizeof(temp3), stdin);
    temp3[strlen(temp3)-1] = '\0';
    fgets(temp4, sizeof(temp4), stdin);
    checkKey(element.key[element.index]);
    temp4[strlen(temp4)-1] = '\0';
    for(int i=0; i<maxSize; i++) {
        element.c1[element.index][i] = temp1[i];
        element.c2[element.index][i] = temp2[i];
        element.c3[element.index][i] = temp3[i];
        element.c4[element.index][i] = temp4[i];
    }
    printf("OK\n");
    element.index++;
}

void print() {
    for(int i=0; i<=element.index; i++) {
        if(element.key[i] != 0) {
            printf("%d\n%s, %s, %s, %s\n", element.key[i], element.c1[i], element.c2[i], element.c3[i], element.c4[i]);
        }
    }
}

int main() {
    int func;
    element.index = 0;
    while(scanf("%d", &func) != EOF) {
        if(func == 0) {
            printf("quit\n");
            break;
        }
        else if(func > 2) {
            printf("ERROR\n");
        }
        else if(func == 1) {
            input();
        }
        else if(func == 2) {
            print();
        }
    }
    return 0;
}