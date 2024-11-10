#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct equNode {
    int coef, expon;
    char var;
    struct equNode *link;
} equNode;
typedef struct equ {
    int size;
    char alph;
    struct equNode *head;
    struct equ *link;
} equ;
typedef equNode * equPointer;
typedef equ * equPtr;

equPointer clear(equPointer *node);
void pushEqu(equPtr *node, int size, char alph, equPointer head);
void pushequNode(equPointer *node, int coef, char var, int expon);
int eread(equPtr *tempEqu, char *input);
void print(equPtr equ);

int main() {
    int equCount = 0;
    char input[100];
    equPtr tempEqu=(equPtr)malloc(sizeof(equ)), temp;
    tempEqu = NULL;
    while (scanf("%s", input) != EOF) {
        if(input[0] == '0') {
            break;
        }
        int index = eread(&tempEqu, input);
        if(index < 0) {
            if(index <= -1) {
                printf("ERROR\n");
            }
            continue;
        }
        else {
            temp = tempEqu;
            while(temp->alph != index) {
                temp = temp->link;
            }
            print(temp);
        }
    }
    printf("quit\n");
    return 0;
}

equPointer clear(equPointer *node) {
    equPointer ptr = *node;
    while(ptr->link) {
        ptr = ptr->link;
        free(*node);
        *node = ptr;
    }
    free(*node);
    *node = NULL;
}

void pushEqu(equPtr *node, int size, char alph, equPointer head) {
    equPtr ptr = *node;
    equPtr tempNode = (equPtr)malloc(sizeof(equ));
    tempNode->size = size;
    tempNode->alph = alph;
    tempNode->head = head;
    tempNode->link = NULL;
    if(*node == NULL) {
        *node = tempNode;
        return;
    }
    while(ptr->link) {
        ptr = ptr->link;
    }
    ptr->link = tempNode;
}

void pushequNode(equPointer *node, int coef, char var, int expon) {
    if(coef == 0) {
        return;
    }
    equPointer ptr = *node;
    equPointer tempNode = (equPointer)malloc(sizeof(equNode));
    tempNode->coef = coef;
    tempNode->expon = expon;
    tempNode->var = var;
    tempNode->link = NULL;
    if(ptr == NULL) {
        *node = tempNode;
        return;
    }
    while(ptr->link) {
        ptr = ptr->link;
    }
    ptr->link = tempNode;
}

int eread(equPtr *tempEqu, char *input) {
    equPointer equ = (equPointer) malloc(sizeof(equNode));
    int preExpon, alphCount;
    char *iptr=input, alph, pre, link, *tempNum, var, preVar, num[15];
    equ = NULL;
    preExpon=2147483647, alphCount=0, preVar=0;
    alph = *iptr;
    if(isalpha(alph) == 0) {
        return -1;
    }
    pre = *iptr++;
    link = *iptr;
    if(link != '=') {
        return -1;
    }
    pre = *iptr++;
    link = *iptr;
    if(link == '\0') {
        return -1;
    }
    int coef = 0;
    int expon = 0;
    while(link != '\0') {
        alphCount++;
        tempNum = num;
        while(1) {
            if(link == '\0') {
                *tempNum = '\0';
                coef = atoi(num);
                break;
            }
            else if(isalpha(tolower(link))) {
                var = tolower(link);
                if(pre == '+' || pre == '=') {
                    coef = 1;
                    break;
                }
                else if( pre == '-' ) {
                    coef = -1;
                    break;
                }
                else {
                    *tempNum = '\0';
                    coef = atoi(num);
                    break;
                }
                pre = *iptr++;
                link = *iptr;
            }
            else if(isdigit(link) || (link=='+'||link =='-')) {
                *tempNum++ = link;
                pre = *iptr++;
                link = *iptr;
            }
            else {
                clear(&equ);
                free(equ);
                return -1;
            }
        }
        if(coef == 0) {
            clear(&equ);
            free(equ);
            return -1;
        }
        if(link != '\0') {
            pre = *iptr++;
            link = *iptr;
        }
        if(link=='\0' && tolower(pre)!='x') {
            var = expon = 0;
        }
        else if(link == '^') {
            pre = *iptr++;
            link = *iptr;
            tempNum = num;
            while(1) {
                if(link=='\0' || (link=='+'||link=='-')) {
                    *tempNum = '\0';
                    expon = atoi(num);
                    break;
                } 
                else if (isdigit(link)) {
                    *tempNum++ = link;
                    pre = *iptr++;
                    link = *iptr;
                }
                else {
                    clear(&equ);
                    free(equ);
                    return -1;
                }
            }
        }
        else if(link=='\0' || (link=='+'||link=='-')) {
            expon = 1;
        }
        else if(isdigit(link) == 0) {
            clear(&equ);
            free(equ);
            return -1;
        }
        if(preExpon < expon || (preExpon==expon && preVar==var)) {
            clear(&equ);
            free(equ);
            return -1;
        }
        preExpon = expon;
        preVar = var;
        pushequNode(&equ, coef, var, expon);
    }
    equPtr temp = *tempEqu;
    if(temp) {
        while(temp) {
            if(temp->alph == alph) {
                break;
            }
            temp = temp->link;
        }
    }
    if(temp) {
        clear(&temp->head);
        temp->head = equ;
        temp->size = alphCount;
    }
    else {
        pushEqu(tempEqu, alphCount, alph, equ);
    }
    return alph;
}

void print(equPtr equ) {
    equPointer ptr = equ->head;
    printf("%c %d\n", equ->alph, equ->size);
    while(ptr != NULL) {
        printf("%d %d", ptr->coef, ptr->expon);
        if(ptr->var) {
            printf(" %c", ptr->var);
        }
        printf("\n");
        ptr = ptr->link;
    }
}