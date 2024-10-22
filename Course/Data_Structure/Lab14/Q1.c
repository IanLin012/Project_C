#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define maxNode 1001
#define maxChar 10001

void inputTree(char*, int*);
bool pathExist(char*, int, char*, int);

int main() {
    char *tree = (char*) malloc(sizeof(char) * maxNode);
    char *input = (char*) malloc(sizeof(char) * maxChar);
    int size = 0;
    while(fgets(input, maxChar, stdin)) {
        switch(input[0]) {
            case '0':
                printf("quit\n");
                return 0;
            case '1':
                inputTree(tree, &size);
                break;
            case '2':
                if(size) {
                    fgets(input, maxChar, stdin);
                    if(pathExist(tree, size, input, 0) == true) {
                        printf("YES\n");
                    }
                    else {
                        printf("NO\n");
                    }
                }
                else {
                    printf("NULL\n");
                }
                break;
        }
    }
    return 0;
}

void inputTree(char *tree, int *size) {
    char *input = (char*) malloc(sizeof(char) * maxChar);
    for(int n=1; n<maxNode; n*=2) {
        fgets(input, maxChar, stdin);
        char *temp = strtok(input, " \n");
        for(int i=0; i<n; i++) {
            if(!strcmp(temp, "end")) {
                *size = n-1; 
                printf("OK\n");
                return;
            }
            if(!strcmp(temp, "99")) {
                tree[n-1+i] = 0;
            }
            else {
                tree[n-1+i] = temp[0];
            }
            temp = strtok(NULL, " \n");
        }
    }
}

bool pathExist(char *tree, int size, char *path, int i) {
    if(*path == '\n') {
        return true;
    }
    if(i >= size) {
        return false;
    }
    if(tree[i] == *path) {
        return(pathExist(tree, size, path+1, i*2+1) || pathExist(tree, size, path+1, i*2+2));
    }
    else {
        return false;
    }
}