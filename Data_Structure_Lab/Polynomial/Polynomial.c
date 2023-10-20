#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <math.h>
#include <limits.h>

#define co coefficient
#define ex exponent
#define altoi(A) ((A)-'A') // alpha to index
#define MAX_POLYS 30       // (A-Z)

typedef struct Polynominal {
    double coefficient;
    int exponent;
    struct Polynominal *nterm; // linked list
} Poly;

/////////////////////
/* other functions */
/////////////////////

// Delete whole list and *head from poly[]
void delList_inArr(char);
// Delete whole list from pointer given
void delList(Poly**);
// Push back to the poly[], create one if none
void pushList_inArr(char, double, int);
// Push back to the pointer (list) given, create one if none
void push(Poly**, double, int);
// Return a substring with a len of n
char *substr(char*, int);
// Print and format Poly for the given pointer (list)
void printfPoly(Poly *);

//////////////////
/* HW functions */
//////////////////
// Enter a poly using format A=... and store it
void readPoly();
// Enter a poly and print it out
void printPoly();
// Enter two polys and add them 
void Padd();
// Enter two polys and multiply them
void Pmult();
// Enter a number and then a poly to caculate the result
void Peval();

// Globle variables 
Poly *polyHead[MAX_POLYS]={0};
Poly *polyBack[MAX_POLYS]={0};

int main() {
    // reads 1-6 for different functionality
    char cmd;
    while(scanf("%c", &cmd)) {
        switch (cmd) {
        case '1':
            readPoly();
            break;
        case '2':
            printPoly();
            break;
        case '3':
            Padd();
            break;
        case '4':
            Pmult();
            break;
        case '5':
            Peval();
            break;
        case '6':
            printf("leave\n");
            return 0;
        }
        // flush unwanted input
        fflush(stdin);
    }
    return 0;
}

void delList_inArr(char i) {
    Poly *cur = polyHead[i], *next;
    // return if the list is empty
    if(cur == NULL) return;
    // free all memories
    while(cur != NULL) {
        next = cur->nterm;
        free(cur);
        cur = next;
    }
    // clear heads and tails
    polyHead[i] = NULL;
    polyBack[i] = NULL;
}

void delList(Poly **p) {
    Poly *cur = *p, *next;
    // return if the list is empty
    if(cur == NULL) return;
    // free all memories
    while(cur != NULL) {
        next = cur->nterm;
        free(cur);
        cur = next;
    }
    *p = NULL;
}

void pushList_inArr(char i, double co, int ex) {
    // create a new node 
    Poly *node = (Poly *)malloc(sizeof(Poly));
    node->co = co;
    node->ex = ex;
    node->nterm = NULL;

    // if there is no node in the list
    if(polyBack[i] == NULL) {
        polyBack[i] = node;
        polyHead[i] = node;
    } else { // append list
        polyBack[i]->nterm = node;
        polyBack[i] = node;
    }

}

void push(Poly **head, double co, int ex) {
    // create a new node
    Poly *node = (Poly *)malloc(sizeof(Poly));
    node->co = co;
    node->ex = ex;
    node->nterm = NULL;

    // if there is no node in the list
    if(*head == NULL) {
        *head = node;
    } else { // append list
        Poly *tmp = *head;
        // find the last element in the list
        while(tmp->nterm != NULL) {
            tmp = tmp->nterm;
        }
        tmp->nterm = node;
    }

}

char *substr(char* str, int n) {
    static char *sub;
    sub = (char *)malloc((n+1) * sizeof(char));
    strncpy(sub, str, n);
    sub[n] = '\0'; // place the null terminator
    return sub;
}

void printfPoly(Poly *head) {
    // do not need to add plus sign if the first term is positive
    char firstTerm = 1;
    Poly *tmp = head;
    // iterate until end of list
    while(tmp != NULL) {
        // if coe. is positive
        if(tmp->co >= 0 && !firstTerm)
            printf("+");
        printf("%.1lf", tmp->co);
        // if not constant (exp. > 0)
        if(tmp->ex != 0)
            printf("x");
        // if exp. > 1
        if(tmp->ex != 0 && tmp->ex != 1)
            printf("^%d", tmp->ex);

        tmp = tmp->nterm;
        firstTerm = 0;
    }

    printf("\n");
}

////////////////////////////////
/* HW METHODS START FROM HERE */
////////////////////////////////

void readPoly() {
    char input[100];
    char *iptr = input;
    char alpha;

    fflush(stdin);
    printf("readPoly():\n");
    scanf("%s", input);

    iptr = input;
    
    // check if first char. is alphabet 
    if(!isalpha(*iptr)) {
        printf("erro1 at - %c\n", *iptr);
        readPoly();
        return;
    } else alpha = *iptr;

    iptr++;

    // check if second char. is equal sign
    if(*iptr != '=') {
        printf("erro2 at - %c\n", *iptr);
        readPoly();
        return;
    }

    iptr++;

    /* PROCESS polynominal */

    char *next;
    double dco=0;
    int iex=0;
    int len=0;
    alpha = altoi(alpha);
    delList_inArr(alpha);

    while(*iptr != '\0') {
        /* PROCESS coefficient */
        next = iptr;
        len=0; // count the length of numbers
        // find next "x"
        while(1) {
            // no "x", rest of the numbers is constant (x^0)
            if(*next == '\0') {
                dco = atof(substr(iptr, len));
                break;
            }
            // found "x"
            if(*next == 'x') {
                // check if term is -x or +x or x (coe. is 1 or -1)
                if((*(next-1) == '-' || *(next-1) == '+') || *(next-1) == '=')
                    dco = (*(next-1) == '-') ? -1 : 1;
                else // coe. is numbers before x
                    dco = atof(substr(iptr, len));
                next++; // points to next term
                break;
            }
            next++, len++;
        }
        
        iptr = next;
        /* PROCESS exponent */
        // is constant
        if(*iptr == '\0' && *(iptr-1) != 'x') {
            iex = 0;
        }
        // find next "^"
        else if(*iptr == '^') {
            iptr++;
            next = iptr;
            len = 0;
            while(1) {
                // end of a term
                if(*next == '\0' || (*next == '+' || *next == '-')) {
                    iex = atoi(substr(iptr, len));
                    break;
                }
                next++, len++;
            }
            iptr = next;
        } 
        // no "^" (x^1)
        else if(*iptr == '\0' || (*iptr == '+' || *iptr == '-')) {
            iex = 1;
        }
        // the input is not illegal 
        else {
            printf("erro3 at - %c", *iptr);
            readPoly();
            return;
        }
        // store data
        pushList_inArr(alpha, dco, iex);

    }
}

void printPoly() {
    char c;
    Poly *node;
    printf("printPoly(): ");
    fflush(stdin);
    scanf("%c", &c);
    c = altoi(c);
    node = polyHead[c];
    // return if the list is empty
    if(node == NULL) {
        printf("Not exist.\n");
        return;
    }
    printfPoly(node);
}

void Padd() {
    char c1, c2;
    fflush(stdin);
    printf("Padd(): ");
    scanf("%c %c", &c1, &c2);
    c1 = altoi(c1), c2 = altoi(c2);
    Poly *ans = NULL, **ansHead = &ans;
    Poly *p1 = polyHead[c1], *p2 = polyHead[c2];
    // return if any of two poly is empty
    if(p1 == NULL || p2 == NULL) {
        printf("The variable(s) is empty!\n");
        return;
    }
    // iterate p1 & p2
    while(p1 != NULL || p2 != NULL) {
        // if one of two poly has reach the end
        if(p1 == NULL) {
            push(&ans, p2->co, p2->ex);
            p2 = p2->nterm;
            continue;
        }
        if(p2 == NULL) {
            push(&ans, p1->co, p1->ex);
            p1 = p1->nterm;
            continue;
        }
        // compare the exp. of two polys
        if(p1->ex > p2->ex) {
            push(&ans, p1->co, p1->ex);
            p1 = p1->nterm;
        }
        if(p1->ex < p2->ex) {
            push(&ans, p2->co, p2->ex);
            p2 = p2->nterm;
        }
        if(p1->ex == p2->ex) {
            push(&ans, (p1->co + p2->co), p1->ex);
            p1 = p1->nterm;
            p2 = p2->nterm;
        }
    }
    printfPoly(*ansHead);
    delList(ansHead);
}

void Pmult() {
    char c1, c2;
    fflush(stdin);
    printf("Pmult(): ");
    scanf("%c %c", &c1, &c2);
    c1 = altoi(c1), c2 = altoi(c2);
    Poly *ans = NULL, **ansHead = &ans;
    Poly *p1 = polyHead[c1], *p2 = polyHead[c2];
    // return if any of two poly is empty
    if(p1 == NULL || p2 == NULL) {
        printf("The variable(s) is empty!\n");
        return;
    }
    double co;
    int ex;
    char notCreate=1;
    // iterate p1
    while(p1 != NULL) {
        // iterate p2
        while(p2 != NULL) {
            co = p1->co * p2->co;
            ex = p1->ex + p2->ex;
            Poly *tmp = *ansHead;
            while((tmp) != NULL || notCreate) { // find if exist
                if(notCreate) {
                    push(&ans, co, ex);
                    notCreate = 0;
                    break;
                }
                if((tmp)->ex == ex) { // found same exp
                    (tmp)->co += co;
                    break;
                }
                if((tmp)->ex < ex) { // insert at first
                    Poly *newp = (Poly *)malloc(sizeof(Poly));
                    newp->co = co, newp->ex = ex;
                    newp->nterm = tmp;
                    *ansHead = newp;
                    break;
                }
                if((tmp)->nterm == NULL) { // insert at last
                    push(&ans, co, ex);
                    break;
                }
                if((tmp)->nterm->ex < ex){ // insert at mid
                    Poly *newp = (Poly *)malloc(sizeof(Poly));
                    newp->co = co, newp->ex = ex;
                    newp->nterm = (tmp)->nterm;
                    (tmp)->nterm = newp;
                    break;
                }
                tmp = tmp->nterm;
            }
            p2 = p2->nterm;
        }
        p1 = p1->nterm;
        p2 = polyHead[c2];
    }
    printfPoly(*ansHead);
    delList(ansHead);
}

void Peval() {
    double x, ans=0;
    char c;
    fflush(stdin);
    printf("Peval(): ");
    scanf("%lf %c", &x, &c);
    c = altoi(c);
    Poly *p = polyHead[c];
    while(p != NULL) {
        ans += p->co * pow(x, p->ex);
        p = p->nterm;
    }
    printf("%.1lf\n", ans);
}