#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

typedef struct treePointer
{
    char data;
    struct treePointer *left, *right;
} treePointer;

treePointer *createTree() {
    treePointer *newNode = (treePointer*)malloc(sizeof(treePointer));
    newNode->left = newNode->right = NULL;
    return newNode;
}

treePointer *logicTree (char *input) {
    treePointer *newTree = createTree();
    int i = 0;
    bool b = false;
    for(i=strlen(input)-1; i>=0; i--) {
        if(input[i] = ')') {
            for(int j=i; j>=0; j--) {
                if(input[j] == '(') {
                    i = j;
                    break;
                }
            }
        }
        if(input[i]=='^' || input[i]=='v') {
            b = true;
            break;
        }   
    }
    if(b == true) {
        newTree->data = input[i];
        input[i] = '\0';
        newTree->left = logicTree(input);
        newTree->right = logicTree(&input[i+1]);
        return newTree;
    }
    if(input[0] == '-') {
        newTree->data = '-';
        newTree->left = logicTree(&input[1]);
        return newTree;
    }
    else if(isalpha(input[0])) {
        newTree->data = input[0];
        return newTree;
    }
    return newTree;
}

void print(treePointer *newTree) {
    printf("%c", newTree->data);
    if(newTree->left != NULL) {
        print(newTree->left);
    }
    if(newTree->right != NULL) {
        print(newTree->right);
    }
}

int main() {
    char input[100];
    treePointer *root;
    while(true) {
        scanf("%s", input);
        if(input[0] == '0') {
            printf("quit");
            break;
        }
        root = createTree();
        root = logicTree(input);
        print(root);
        printf("\n");
    }
    return 0;
}