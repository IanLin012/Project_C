#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define maxNode 1000

void *inputTree(char *tree) {
    char *input = (char*) malloc(sizeof(char) * maxNode);
    for(int i=0; i<maxNode; i++) {
        scanf("%s", &input);
        if(input[i] == 'e') {
            break;
        }
    }
}

void sumExist() {
}

int main() {
    int func;
    char tree[maxNode];
    while(scanf("%d", &func) != EOF) {
        if(func == 0) {
            printf("quit\n");
            break;
        }
        else if(func == 1) {
            inputTree(tree);
        }
        else if(func == 2) {
            sumExist();
        }
    }
}