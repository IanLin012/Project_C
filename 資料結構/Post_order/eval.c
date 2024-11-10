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
    double *stack;
} DoubleStack;

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

DoubleStack *newDoubleStack() {
    
    DoubleStack *stack = (DoubleStack*)malloc(sizeof(DoubleStack));
    if(!stack) {
        free(stack);
        return 0;
    }
    stack->top = 0;
    stack->stack = (double*)malloc(sizeof(double) * MAX_CHAR);
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
void pushD(DoubleStack *st, double c) {
    if(st->top < MAX_CHAR-1)
        st->stack[st->top++] = c;
}

char *popC(CharStack *st) {
    return &st->stack[--st->top];
}
double popD(DoubleStack *st) {
    return st->stack[--st->top];
}

char topC(CharStack *st) {
    return st->stack[st->top-1];
}
double topD(DoubleStack *st) {
    return st->stack[st->top-1];
}

int precedence(char c) {
    if(c == '(') return 0;
    if(c == '+' || c == '-') return 1;
    if(c == '*' || c == '/') return 2;
    if(c == '^') return 3;
    return -1;
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

void voidintoPostfix(char *infix, char *post) {
    strcpy(post, "");
    int flag = 1;
    char *c = infix, latest = *c;
    CharStack *s = newCharStack();
    while(*c != '\0') {
        if(isalnum(*c)) {
            if(isalnum(latest) && *c != *infix) {
                flag = 0;
                break;
            }
            strncat(post, c, 1);
        } else if(*c == '(') {
            pushC(s, *c);
        } else if(*c == ')') {
            while(s->top && topC(s)!='(') {
                strncat(post, popC(s), 1);
            }
            if(topC(s) != '(') {
                flag = 0;
                break;
            }
            else {
                popC(s);
            }
        }
        else if(precedence(*c) < 0) {
            flag = 0;
            break;
        }
        else {
            if(!isalnum(latest) && *c != *infix && precedence(latest) > 0) {
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

double doubleeval(char *infix, int *vals) {
    char *postfix = (char*) malloc(sizeof(char) * MAX_CHAR);
    voidintoPostfix(infix, postfix);
    if(!strcmp(postfix, "ERROR")) {
        return 0;
    }
    DoubleStack *s = newDoubleStack();
    for(char *c=postfix; *c != '\0'; c++) {
        if(isalpha(*c)) {
            pushD(s, vals[*c]);
        }
        if(isdigit(*c)) {
            pushD(s, *c-'0');
        }

        double a,b;
        if(*c == '+') {
            a=b=0;
            if(s->top)
                a = popD(s);
            if(c-2 >= postfix)
                b = popD(s);
            pushD(s, b+a);
        }
        if(*c == '-') {
            a=b=0;
            if(s->top)
                a = popD(s);
            if(c-2 >= postfix)
                b = popD(s);
            pushD(s, b-a);
        }
        if(*c == '*') {
            a=b=0;
            if(s->top)
                a = popD(s);
            if(s->top)
                b = popD(s);
            pushD(s, b*a);
        }
        if(*c == '/') {
            a=b=0;
            if(s->top)
                a = popD(s);
            if(s->top)
                b = popD(s);
            pushD(s, b/a);
        }
        if(*c == '^') {
            a=b=0;
            if(s->top)
                a = popD(s);
            if(s->top)
                b = popD(s);
            pushD(s, pow(b, a));
        }
    }
    if(s->top) {
        return popD(s);
    } else {
        printf("Error evaling\n");
        return 0;
    }
}

int main() {
    char *in  = (char*) malloc(sizeof(char) * MAX_CHAR);
    char *post = (char*) malloc(sizeof(char) * MAX_CHAR);
    int  *vals = (int*) malloc(sizeof(int) * 255);
    while(scanf("%s", in) != EOF) {
        voidintoPostfix(in, post);
        voidgetValue(in, vals);
        printf("eval:%.1lf\n", doubleeval(in, vals));
    }
    return 0;
}