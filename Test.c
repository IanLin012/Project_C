#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_NODES 1000
#define MAX_CHARS 10000+1

void inputTree(int *tree, int *size) {
    char *input = (char*) malloc(sizeof(char) * MAX_CHARS);
    for (int n=1; n<MAX_NODES; n<<=1) {
        fgets(input, MAX_CHARS, stdin);
        char *tmp = strtok(input, " \n");
        for (int i=0; i<n; i++) {
            if (tmp[0] == 'e') {
                *size = n-1; 
                printf("OK\n");
                return;
            }
            if (tmp[0] == 'x') {
                tree[n-1+i] = 0;
            } else {
                tree[n-1+i] = atoi(tmp);
            }
            tmp = strtok(NULL, " \n");
        }
    }
    printf("MAX NODES REACHED\n");
}

void printTree(int *tree, int size) {
    for (int n=1; n<=size; n<<=1) {
        for (int i=0; i<n; i++) {
            printf("%d ", tree[n-1+i]);
        }
        printf("\n");
    }
}

int checkSum(int *tree, int size, int i, int sum) {
    if (sum == 0) return 1;
    if (sum < 0 || i >= size) return 0;
    return checkSum(tree, size, i*2+1, sum-tree[i]) ||
           checkSum(tree, size, i*2+2, sum-tree[i]) ;
}

int main() {

    int *tree = (int*) malloc(sizeof(int) * MAX_NODES);
    int size=0;

    char *input = (char*)malloc(sizeof(char) * 15);
    while (fgets(input, 15, stdin)) {
        switch (input[0]) {
            case '1':
                inputTree(tree, &size);
                break;
            case '2':
                if (!size) {
                    printf("ERROR\n");
                    break;
                }
                fgets(input, 15, stdin);
                printf("%s\n", checkSum(tree, size, 0, atoi(input)) ? "YES" : "NO");
                break;
            case '0':
                printf("quit\n");
                return 0;
        }
    }
    inputTree(tree, &size);
    

    return 0;
}