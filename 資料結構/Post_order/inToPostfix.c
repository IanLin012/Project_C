#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_CHAR 100

typedef struct {
    int top, size;
    char *stack;
} Stack;

Stack *newStack(size_t s) {
    Stack *stack = (Stack*)malloc(sizeof(Stack));
    if(!stack) {
        free(stack);
        return 0;
    }
    stack->top = 0;
    stack->size = s;
    stack->stack = (char*)malloc(sizeof(char) * s);
    if(!stack->stack) {
        free(stack);
        return 0;
    }
    return stack;
}

void push(Stack *st, char c) {
    if(st->top < st->size-1)
        st->stack[st->top++] = c;
}

char *pop(Stack *st) {
    return &st->stack[--st->top];
}

char top(Stack *st) {
    return st->stack[st->top-1];
}

int precedence(char c) {
    if(c == '(') return 0;
    if(c == '+' || c == '-') return 1;
    if(c == '*' || c == '/') return 2;
    if(c == '^') return 3;
    return -1;
}

void voidintoPostfix(char *str, char *post) {
    strcpy(post, "");
    int flag = 1;
    char *c = str, latest = *c;
    Stack *s = newStack(MAX_CHAR);
    while(*c != '\0') {
        if(isalnum(*c)) {
            if(isalnum(latest) && *c != *str) {
                flag = 0;
                break;
            }
            strncat(post, c, 1);
        }
        else if(*c == '(') {
            push(s, *c);
        }
        else if(*c == ')') {
            while(s->top && top(s)!='(') {
                strncat(post, pop(s), 1);
            }
            if(top(s) != '(') {
                flag = 0;
                break;
            }
            else {
                pop(s);
            }
        }
        else if(precedence(*c) < 0) {
            flag = 0;
            break;
        }
        else {
            if(!isalnum(latest) && *c != *str && precedence(latest) > 0) {
                flag = 0;
                break;
            }
            while(s->top && precedence(top(s)) >= precedence(*c)) {
                strncat(post, pop(s), 1);
            }
            push(s, *c);
        }
        latest = *(c++);
    }
    if(!flag) {
        strcpy(post, "ERROR");
        return;
    }
    while(s->top) {
        strncat(post, pop(s), 1);
    }
} 

int main() {
    char *str  = (char*) malloc(sizeof(char) * MAX_CHAR);
    char *post = (char*) malloc(sizeof(char) * MAX_CHAR);
    int skipLines = 1;
    while(scanf("%s", str) != EOF) {
        voidintoPostfix(str, post);
        printf("%s%s\n\n", (strcmp(post, "ERROR") ? "Postfix expression:" : "") , post);
    }
    return 0;
}