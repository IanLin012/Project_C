#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define MAX_CHAR 100

typedef struct {
    int top;
    char *stack;
} CharStack;

typedef struct {
    int top;
    int *stack;
} IntStack;

CharStack *newCharStack() {
    CharStack *stack = (CharStack*)malloc(sizeof(CharStack));
    if(!stack) {
        free(stack);
        return 0;
    }
    stack->top = 0;
    stack->stack = (char*)malloc(sizeof(char) * MAX_CHAR);
    if(!stack->stack) {
        free(stack);
        return 0;
    }
    return stack;
}

IntStack *newIntStack() {
    IntStack *stack = (IntStack*)malloc(sizeof(IntStack));
    if(!stack) {
        free(stack);
        return 0;
    }
    stack->top = 0;
    stack->stack = (int*)malloc(sizeof(int) * MAX_CHAR);
    if(!stack->stack) {
        free(stack);
        return 0;
    }
    return stack;
}

void pushC(CharStack *st, char c) {
    if(st->top < MAX_CHAR-1)
        st->stack[st->top++] = c;
}
void pushI(IntStack *st, int c) {
    if(st->top < MAX_CHAR-1)
        st->stack[st->top++] = c;
}

char *popC(CharStack *st) {
    return &st->stack[--st->top];
}
int popI(IntStack *st) {
    return st->stack[--st->top];
}

char topC(CharStack *st) {
    return st->stack[st->top-1];
}
int topI(IntStack *st) {
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
    CharStack *s = newCharStack();
    while(*c != '\0') {
        if(isalnum(*c)) {
            if(isalnum(latest) && *c != *str) {
                flag = 0;
                break;
            }
            strncat(post, c, 1);
        }
        else if(*c == '(') {
            pushC(s, *c);
        }
        else if(*c == ')') {
            while(s->top && topC(s)!='(') {
                strncat(post, popC(s), 1);
            }
            if(topC(s) != '(') {
                flag = 0;
                break;
            } else {
                popC(s);
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
            while(s->top && precedence(topC(s)) >= precedence(*c)) {
                strncat(post, popC(s), 1);
            }
            pushC(s, *c);
        }
        latest = *(c++);
    }
    if(!flag) {
        strcpy(post, "ERROR");
        return;
    }
    while(s->top) {
        strncat(post, popC(s), 1);
    }
} 

void voidgetValue(char *infix, int *vals) {
    int *hasVal = (int*) malloc(sizeof(int) * 255);
    memset(hasVal, 0, sizeof(int) * 255);
    for(char *c=infix; *c != '\0'; c++) {
        if(isalpha(*c) && !hasVal[*c]) {
            scanf("%d", &vals[*c]);
            hasVal[*c] = 1;
        }
    }
}

int main() {
    char *in  = (char*) malloc(sizeof(char) * MAX_CHAR);
    char *post = (char*) malloc(sizeof(char) * MAX_CHAR);
    int  *vals = (int*) malloc(sizeof(int) * 255);
    int  *flag = (int*) malloc(sizeof(int) * 255);
    while(scanf("%s", in) != EOF) {
        memset(flag, 0, sizeof(int) * 255);
        voidgetValue(in, vals);
        printf("infix expression:%s\n", in);
        for(char *c=in; *c != '\0'; c++) {
            if(isalpha(*c) && !flag[*c]) {
                printf("%c=%d\n", *c, vals[*c]);
                flag[*c] = 1;
            }
        }
    }
    return 0;
}