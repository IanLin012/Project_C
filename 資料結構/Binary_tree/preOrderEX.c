#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

typedef struct Node *treePointer;
typedef struct Node {
    char data;
    treePointer leftChild;
    treePointer rightChild;
} Node;
treePointer newTree() {
    treePointer ptr;
    ptr = (treePointer) malloc(sizeof(Node));
    ptr->leftChild = NULL;
    ptr->rightChild = NULL;
    return ptr;
}
treePointer logicTree(char *str) {
    treePointer ptr = newTree();
    int n = strlen(str);
    int i;
    bool found = false;
    for(i=n-1; i>=0; i--) {
        if(str[i] == ')') for(int j=i; j>=0; j--) {
            if(str[j] == '(') {
                i=j;
                break;
            }
        }
        if(str[i]=='^' || str[i]=='v') {
            found = true;
            break;
        }
    }
    if(found) {
        ptr->data = str[i];
        str[i] = '\0';
        ptr->leftChild = logicTree(str);
        ptr->rightChild = logicTree(&str[i+1]);
        return ptr;
    }
    if(str[n-1] == ')') for(i=n-1; i>=0; i--) {
        if(str[i] == '(') {
            str[n-1] = '\0';
            if(str[i-1] == '-') {
                ptr->data = '-';
                str[i] = '\0';
                ptr->leftChild = logicTree(&str[i+1]);
                return ptr;
            }
            else {
                str[i] = '\0';
                ptr = logicTree(&str[i+1]);
                return ptr;
            }
        }
    }
    if(str[0]=='-') {
        ptr->data = '-';
        ptr->leftChild = logicTree(&str[1]);
        return ptr;
    }
    else if(isalpha(str[0])) {
        ptr->data = str[0];
        return ptr;
    }
    return ptr;
}
void print(treePointer ptr) {
    printf("%c", ptr->data);
    if(ptr->leftChild!=NULL) {
        print(ptr->leftChild);
    }
    if(ptr->rightChild!=NULL) {
        print(ptr->rightChild);
    }
}
int main() {
    char str[100];
    treePointer parent;
    while(true) {
        scanf("%s",str);
        if(str[0] == '0') {
            break;
        }
        parent = newTree();
        parent = logicTree(str);
        print(parent);
        printf("\n");
    }
    printf("quit\n");
}