#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_POLYS 10

typedef struct equNode {
    int coe;
    char var;
    int exp;
    struct equNode *next;
} equNode;
typedef struct equ {
    int size;
    char name;
    struct equNode *head;
    struct equ *next;
} equ;
typedef equNode * equPointer;
typedef equ * equPtr;

equPointer clearEqNodes(equPointer *node);
void pushEq(equPtr *node, int size, char name, equPointer head);
void pushEqNode(equPointer *node, int coe, char var, int exp);
int readEq(equPtr *eqs, char *input);

equPointer clearEqNodes(equPointer *node) {
    equPointer nptr = *node;
    while(nptr->next) {
        nptr = nptr->next;
        free(*node);
        *node = nptr;
    }
    free(*node);
    *node = NULL;
}

void pushEq(equPtr *node, int size, char name, equPointer head) {
    equPtr nptr = *node;
    equPtr newNode = (equPtr) malloc(sizeof(equ));
    newNode->name = name;
    newNode->size = size;
    newNode->next = NULL;
    newNode->head = head;

    if(!*node) {
        *node = newNode;
        return;
    }

    while(nptr->next) {
        nptr = nptr->next;
    }
    nptr->next = newNode;
}

void pushEqNode(equPointer *node, int coe, char var, int exp) {
    if(coe == 0) return;

    equPointer nptr = *node;
    equPointer newNode = (equPointer) malloc( sizeof(equNode) );
    newNode->coe = coe;
    newNode->var = var;
    newNode->exp = exp;
    newNode->next = NULL;

    if(!nptr) {
        *node = newNode;
        // printf("BREAKPOINT1 %p\n", node);
        return;
    }

    while(nptr->next) {
        nptr = nptr->next;
    }
    nptr->next = newNode;
}

int readEq(equPtr *eqs, char *input) {
    equPointer equ = (equPointer) malloc(sizeof(equNode));
    int prevExp;
    int termCount;
    char *iptr = input;
    char name, prev, next, num[15], *nump, var, prevVar;

    equ = NULL;
    prevExp=2147483647, termCount=0, prevVar=0;

    name = *iptr;
    if(!isalpha(name)) {
        return -1;
    }
    
    prev = *iptr++;
    next = *iptr;

    if(next != '=') {
        return -1;
    }
    prev = *iptr++;
    next = *iptr;

    if(next == '\0') {
        return -1;
    }
    /* PROCESS equation */
    int coe=0;
    int exp=0;

    while(next != '\0') {
        termCount++;

        /* PROCESS coefficient */
        nump = num;
        while(1) {
            if(next == '\0') {
                *nump = '\0';
                coe = atoi(num);
                break;
            }
            else if(isalpha(tolower(next))) {
                // check if term is -x or +x or x (coe. is 1 or -1)
                var = tolower(next);
                if(prev == '+' || prev == '=') {
                    coe = 1;
                    break;
                }
                else if( prev == '-' ) {
                    coe = -1;
                    break;
                }
                else {
                    *nump = '\0';
                    coe = atoi(num);
                    break;
                }
                prev = *iptr++;
                next = *iptr;
            }
            else if(isdigit(next) || (next == '+' || next == '-')) {
                *nump++ = next;
                prev = *iptr++;
                next = *iptr;
            }
            else {
                clearEqNodes(&equ);
                free(equ);
                return -1;
            }
        }

        if(coe == 0) { // coe. can't be 0
            clearEqNodes(&equ);
            free(equ);
            return -1;
        }
        
        if(next != '\0') {
            prev = *iptr++;
            next = *iptr;
        }

        /* PROCESS exponent */
        // is constant
        if(next == '\0' && tolower(prev) != 'x') {
            var = exp = 0;
        }
        else if(next == '^') {
            prev = *iptr++;
            next = *iptr;
            nump = num;
            while(1) {
                if(next == '\0' || (next == '+' || next == '-')) {
                    *nump = '\0';
                    exp = atoi(num);
                    break;
                } 
                else if (isdigit(next)) {
                    *nump++ = next;
                    prev = *iptr++;
                    next = *iptr;
                }
                else {
                    clearEqNodes(&equ);
                    free(equ);
                    return -1;
                }
            }
        }
        else if(next == '\0' || (next == '+' || next == '-')) {
            exp = 1;
        }
        else if(!isdigit(next)) {
            clearEqNodes(&equ);
            free(equ);
            return -1;
        }

        if(prevExp < exp || (prevExp == exp && prevVar == var)) {
            clearEqNodes(&equ);
            free(equ);
            return -1;
        }
        prevExp = exp;
        prevVar = var;
        // store data
        pushEqNode(&equ, coe, var, exp);
    }
    // push to eqs
    equPtr tmp = *eqs;
    if(tmp) {
        while(tmp) {
            // record tmp if found
            if(tmp->name == name) break;
            // move tmp to NULL if not found
            tmp = tmp->next;
        }
    }
    if(tmp) { // found
        clearEqNodes(&tmp->head);
        tmp->head = equ;
        tmp->size = termCount;
    } else { // not found
        pushEq(eqs, termCount, name, equ);
    }
    return name; // sucess
}

void printData(equPtr equ) {
    equPointer nptr = equ->head;
    printf("%c %d\n", equ->name, equ->size);
    while(nptr != NULL) {
        printf("%d %d", nptr->coe, nptr->exp);
        if(nptr->var) {
            printf(" %c", nptr->var);
        }
        printf("\n");
        nptr = nptr->next;
    }
}

void printEq(equPointer head) {
    int coe, exp;
    char var;
    equPointer nptr = head;
    while(nptr != NULL) {
        coe = nptr->coe;
        exp = nptr->exp;
        var = nptr->var;
        if(coe == 1) {
            if(nptr == head) {
                printf("");
            } else if (exp == 0) {
                printf("+1");
            } else {
                printf("+");
            }
        }
        else if(coe == -1) {
            if(exp == 0) {
                printf("-1");
            } else {
                printf("-");
            }
        } 
        else {
            if(nptr == head) {
                printf("%d", coe);
            } else {
                printf("%s%d", (coe > 0 ? "+" : ""), coe);
                
            }
        }

        if(exp == 0) {
            printf("");
        }
        else if(exp == 1) {
            printf("%c", var);
        } 
        else {
            printf("%c^%d", var, exp);
        }

        nptr = nptr->next;
    }
    printf("\n");
}

int main() {
    int eqCount = 0;
    equPtr eqs = (equPtr) malloc(sizeof(equ)), tmp;
    eqs = NULL;

    int cmd;
    char input[100];
    while (scanf("%d", &cmd)) {
        if(cmd == 0) {
            break;
        }
        if(cmd == 1) {
            scanf("%s", input);
            int idx = readEq(&eqs, input);
            if(idx < 0) {
                if(idx <= -1)
                    printf("ERROR\n");
                continue;
            } else {
                tmp = eqs;
                while(tmp->name != idx) {
                    tmp = tmp->next;
                }
                printData(tmp);
            }
        }
        if(cmd == 2) {
            scanf("%s", input);
            tmp = eqs;
            while(tmp->name != *input && tmp->next) {
                tmp = tmp->next;
            }
            if(tmp->name != *input) {
                printf("NO EQUATION\n");
            } else {
                printf("%c=", *input);
                printEq(tmp->head);
            }
        }

    }
    printf("quit\n");

    return 0;
}