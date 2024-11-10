#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

typedef struct logicTree {
    char ch;
    struct logicTree *right, *left;
} logicTree;
logicTree *newNode(char ch) {
    logicTree *newNode = (logicTree*) malloc(sizeof(logicTree));
    newNode->ch = ch;
    newNode->right = newNode->left = NULL;
    return newNode;
}
logicTree* stackTree[100];
int lastsTree=0;
void pushTree(logicTree* element) {
    stackTree[lastsTree++] = element;
}
logicTree *popTree() {
    if (lastsTree) {
        return stackTree[--lastsTree];
    }
    else {
        return NULL;
    }
}
logicTree *topTree() {
    if (lastsTree) {
        return stackTree[lastsTree-1];
    }
    else {
        return NULL;
    }
}
char stackChar[100];
int lastsChar = 0;
void pushChar(char c) {
    stackChar[lastsChar++] = c;
}
char popChar() {
    if (lastsChar) {
        return stackChar[--lastsChar];
    }
    else {
        return 0;
    }
}
char topChar() {
    if (lastsChar) {
        return stackChar[lastsChar-1];
    }
    else {
        return 0;
    }
}
logicTree *makeTree(char* str) {
    int p[255] = {0};
    p['('] = 1;
    p['^'] = p['v'] = 2;
    p['-'] = 3;
    lastsChar = lastsTree = 0;
    char op;
    for (char* c = str; *c != '\0'; c++) {
        if (isalpha(*c) && !p[*c]) {
            pushTree(newNode(*c));
        }
        else if (*c == '(') {
            pushChar(*c);
        }
        else if (*c == ')') {
            while (op = popChar()) {
                if (op == '(') break;
                logicTree *new = newNode(op);
                new->right = popTree();
                new->left = popTree();
                pushTree(new);
            }
        }
        else {
            if (p[*c] == 0) {
                return NULL;
            }
            bool unary = false;
            if (*c == '-' && (c == str ||  p[*(c-1)] )) {
                unary = true;
            }
            while (op = topChar()) {
                if (p[op] < p[*c]) break;
                logicTree* new = newNode(op);
                new->right = popTree();
                new->left = popTree();
                pushTree(new);
                popChar();
            }
            if(unary) {
                pushTree(NULL);
            }
            pushChar(*c);
        }
    }
    while (op = popChar()) {
        logicTree* new = newNode(op);
        new->right = popTree();
        new->left = popTree();
        pushTree(new);
    }
    return topTree();
}
int height(logicTree* head) {
    if (!head) {
        return 0;
    }
    int lheight = height(head->left);
    int rheight = height(head->right);
    return (lheight > rheight) ? lheight+1 : rheight+1;
}
void printCurrentLevel(logicTree* head, int level) {
    if (!head)
        return;
    if (level == 1)
        printf("%c", head->ch);
    else if (level > 1) {
        printCurrentLevel(head->left, level-1);
        printCurrentLevel(head->right, level-1);
    }
}
void printTreeLevelOrder(logicTree* head) {
    for (int i=1; i <= height(head); i++)
        printCurrentLevel(head, i);
}
void printTreePreOrder(logicTree* head) {
    if(!head) {
        return;
    }
    printf("%c", head->ch);
    printTreePreOrder(head->left);
    printTreePreOrder(head->right);
}
void printTreePostOrder(logicTree* head) {
    if(!head) {
        return;
    }
    printTreePostOrder(head->left);
    printTreePostOrder(head->right);
    printf("%c", head->ch);
}
int main() {
    char str[100];
    while(scanf("%s", str)) {
        if(*str == '0') {
            break;
        }
        logicTree* logicTree = makeTree(str);
        printTreeLevelOrder(logicTree);
        printf("\n");
    }
    printf("quit");
    return 0;
}