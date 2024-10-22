#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_POLYS 10
#define MAX 100

typedef struct polyNode *polyPointer;
typedef struct polyNode {
    int coef;
    int expo;
    polyPointer link;
} polyNode;

int POLYCount = 0;
char POLYNames[MAX_POLYS];
polyPointer POLYS[MAX_POLYS];

void pread(char *c) {
    int index;
    bool New = true;
    for (int i = 0; i < MAX_POLYS; i++) {
        if (c[0] == POLYNames[i]) {
            index = i;
            New = false;
            break;
        }
    }
    polyPointer NewPoly = (polyPointer) malloc(sizeof(polyNode));
    NewPoly->link = NULL;
    int p = 2;
    int prev = -1;
    while (c[p] != '\0') {
        int plus = 1;
        if(c[p] == '+'){
            p++;
        }
        else if (c[p] == '-'){
            plus = 0;
            p++;
        }
        int coef = 0;
        while ('0' <= c[p] && c[p] <= '9') {
            coef = coef * 10 + (c[p] - '0');
            p++;
        }
        int expo = 0;
        if (c[p] == 'x' || c[p] == 'X') {
            p++;
            if (c[p] == '^') {
                p++;
                while ('0' <= c[p] && c[p] <= '9') {
                    expo = expo * 10 + (c[p] - '0');
                    p++;
                }
            }
            else {
                expo = 1;
            }
        }
        if(prev == -1){
            prev = expo;
        }
        else {
            if (prev < expo) {
                printf("ERROR\n");
                free(NewPoly);
                return;
            }
            prev = expo;
        }
        if (coef == 0 && expo != 0) {
            coef = 1;
        }
        if (!plus) {
            coef *= -1;
        }
        polyPointer NewNode = (polyPointer) malloc(sizeof(polyNode));
        NewNode->coef = coef;
        NewNode->expo = expo;
        NewNode->link = NULL;
        if (NewPoly->link == NULL) {
            NewPoly->link = NewNode;
        }
        else {
            polyPointer temp = NewPoly;
            while (temp->link != NULL) {
                temp = temp->link;
            }
            temp->link = NewNode;
        }
    }
    if(New) {
        if (POLYCount == MAX_POLYS) {
            printf("FULL\n");
            free(NewPoly);
            return;
        }
        else {
            index = POLYCount;
            POLYNames[POLYCount] = c[0];
            POLYCount++;
        }
    }
    else {
        polyPointer OldPoly = POLYS[index];
        while (OldPoly->link != NULL) {
            polyPointer NextPoly = OldPoly->link;
            free(OldPoly);
            OldPoly = NextPoly;
        }
    }
    POLYS[index] = NewPoly;
    polyPointer OutputPoly = POLYS[index]->link;
    printf("%c\n", POLYNames[index]);
    while (OutputPoly != NULL) {
        printf("%d %d\n", OutputPoly->coef, OutputPoly->expo);
        OutputPoly = OutputPoly->link;
    }
}

void printPoly(polyPointer OutputPoly) {
    int times = 0;
    while (OutputPoly != NULL) {
        int coef = OutputPoly->coef;
        int expo = OutputPoly->expo;
        OutputPoly = OutputPoly->link;

        if (coef == 0) {
            if(times != 0) {
                printf("+");
            }
            printf("%d", coef);
        }
        else if (coef > 0 && expo == 0) {
            if(times != 0) {
                printf("+");
            }
            printf("%d", coef);
        }
        else if (coef < 0 && expo == 0) {
            printf("%d", coef);
        }
        else if (coef == 1 && expo == 1) {
            if(times != 0) {
                printf("+");
            }
            printf("x");
        }
        else if (coef == -1 && expo == 1) {
            printf("-x");
        }
        else if (coef > 1 && expo == 1) {
            if(times != 0) {
                printf("+");
            }
            printf("%dx", coef);
        }
        else if (coef < -1 && expo == 1) {
            printf("%dx", coef);
        }
        else if (coef == 1 && expo > 1) {
            if(times != 0) {
                printf("+");
            }
            printf("x^%d", expo);
        }
        else if (coef == -1 && expo > 1) {
            printf("-x^%d", expo);
        }
        else if (coef > 1 && expo > 1) {
            if(times != 0) {
                printf("+");
            }
            printf("%dx^%d", coef, expo);
        }
        else if (coef < -1 && expo > 1) {
            printf("%dx^%d", coef, expo);
        }
        times++;
    }
    puts("");
}

void printAllPolys() {
    for (int i = 0; i < POLYCount; i++) {
        printf("%c=",POLYNames[i]);
        printPoly(POLYS[i]->link);
    }
    if (POLYCount == 0) {
        printf("NO POLY\n");
    }
}

polyPointer padd(polyPointer a, polyPointer b) {
    polyPointer NewPoly = (polyPointer)malloc(sizeof(polyNode));
    NewPoly->link = NULL;
    while (a != NULL || b != NULL) {
        int coef, expo;
        if (a == NULL) {
            coef = b->coef;
            expo = b->expo;
            b = b->link;
        }
        else if (b == NULL) {
            coef = a->coef;
            expo = a->expo;
            a = a->link;
        }
        else if (a->expo == b->expo) {
            coef = a->coef + b->coef;
            expo = a->expo;
            a = a->link;
            b = b->link;
        }
        else if (a->expo > b->expo) {
            coef = a->coef;
            expo = a->expo;
            a = a->link;
        }
        else if (a->expo < b->expo) {
            coef = b->coef;
            expo = b->expo;
            b = b->link;
        }
        if (coef != 0) {
            polyPointer NewNode = (polyPointer)malloc(sizeof(polyNode));
            NewNode->coef = coef;
            NewNode->expo = expo;
            NewNode->link = NULL;

            if (NewPoly->link == NULL) {
                NewPoly->link = NewNode;
            }
            else {
                polyPointer temp = NewPoly;
                while (temp->link != NULL) {
                    temp = temp->link;
                }
                temp->link = NewNode;
            }
        }
    }
    return NewPoly;
}

int main() {
    int choice;
    char c[MAX];
    char a;
    while (scanf("%d", &choice) != EOF) {
        if (choice == 0) {
            printf("quit\n");
            break;
        }
        else if (choice == 1) {
            scanf("%s", c);
            pread(c);
        }
        else if (choice == 2) {
            scanf(" %c", &a);
            int index = -1;
            for (int i = 0; i < MAX_POLYS; i++){
                if (a == POLYNames[i]) {
                index = i;
                break;
                }
            }
            if (index == -1) {
                printf("NO POLY\n");
            }
            else {
                printf("%c=", POLYNames[index]);
                printPoly(POLYS[index]->link);
            }
        }
        else if (choice == 3) {
            printAllPolys();
        }
        else if (choice == 4) {
            scanf("%s",c);
            int redex, index = -1, a, b;
            bool New = true;
            if (c[1] == '=') {
                for (int i = 0; i < MAX_POLYS; i++) {
                    if (c[0] == POLYNames[i]) {
                        New = false;
                        redex = i;
                        break;
                    }
                }
                if (New) {
                    if (POLYCount == MAX_POLYS) {
                        printf("FULL\n");
                        continue;
                    }
                    else {
                        redex = POLYCount;
                    }
                }
                a = 2;
                b = 4;
            }
            else {
                a = 0;
                b = 2;
            }
            index = -1;
            for (int i = 0; i < MAX_POLYS; i++) {
                if (c[a] == POLYNames[i]) {
                    index = i;
                    break;
                }
            }
            if (index == -1) {
                printf("ERROR\n");
                continue;
            }
            polyPointer ptr_a = POLYS[index]->link;
            index = -1;
            for (int i = 0; i < MAX_POLYS; i++) {
                if (c[b] == POLYNames[i]) {
                    index = i;
                    break;
                }
            }
            if (index == -1) {
                printf("ERROR\n");
                continue;
            }
            polyPointer ptr_b = POLYS[index]->link;
            if (c[1] == '=') {
                if (New) {
                    POLYNames[POLYCount] = c[0];
                    POLYCount++;
                }
                POLYS[redex] = padd(ptr_a, ptr_b);
                printf("%c=", POLYNames[redex]);
                printPoly(POLYS[redex]->link);
            }
            else {
                polyPointer result = padd(ptr_a, ptr_b);
                printf("%s=", c);
                printPoly(result->link);
                free(result);
            }
        }
    } 
    return 0;
}