#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

typedef struct func {
    int coe, idx;
    char var;
    struct func *next;
} func;

typedef struct funcPtr {
    int n;
    struct func *head;
    struct funcPtr *next;
} funcPtr;

char arr[10][10];
funcPtr *Phead = 0, *Plast = 0;

bool isNumber(const char *str);
int segmentation(char * poly);
bool debug();
func *anapoly(bool ERROR);

int main() {
    while(1) {
        bool ERROR = false;
        char poly[256] = {"\0"};
        scanf("%s", poly);
        if (poly[0] == '0') break;
        funcPtr *new = malloc(sizeof(funcPtr));
        new->next = 0;
        new->n = segmentation(poly);
        ERROR = debug();
        new->head = anapoly(ERROR);
        if (!Phead) {
            Plast = new;
            Phead = new;
        }
        else {
            Plast->next = new;
            Plast = new;
        }
    }
    printf("quit\n");
    return 0;
}

bool isNumber(const char *str) {
    if (str==NULL || *str=='\0') {
        return false;
    }
    if (*str=='+' || *str=='-') {
        str++;
    }
    bool hasDot = false;
    bool hasDigit = false;
    while (*str != '\0') {
        if(*str>='0' && *str<='9') {
            hasDigit = true;
        }
        else if(*str == '.') {
            if(hasDot == true) {
                return false;
            }
            hasDot = true;
        }
        else {
            return false;
        }
        str++;
    }
    return hasDigit;
}

int segmentation(char * poly) {
    int j = 0, k;
    for (int i=0; i<10; i++) {
        for (int j=0; j<10; j++) {
            arr[i][j] = '\0';
        }
    }
    for (int i=0; poly[i]!='\0'; i++) {
        k = 0;
        if(poly[i] == '-') {
            arr[j][k] = '-';
            k++;
            i++;
        }
        for ( ; poly[i]!='*' && poly[i]!='\0'; i++) {
            arr[j][k] = poly[i];
            k++;
        }
        if (poly[i+1] == '-') {
            i++;
        }
        j++;
    }
    return j;
}

bool debug() {
    for(int i=0; arr[i][0]!='\0'; i++) {
        int k = 0;
        for(int j=0; arr[i][j]!='\0'; j++)
            if(arr[i][j]=='x' || arr[i][j]=='y' || arr[i][j]=='z') {
                k++;
            }
        if(k > 1) {
            return true;
        }
    }
    return false;
}

func *anapoly(bool ERROR) {
    func *Nhead = 0, *Nlast = 0;
    for(int i=0; arr[i][0]!='\0'; i++){
        func *Nnew = malloc(sizeof(func));
        Nnew->next = 0;
        char coe[100] = {'\0'}, idx[100] = {'\0'};
        int j = 0;
        for( ; arr[i][j] != 'x' && arr[i][j] != 'y' && arr[i][j] != 'z' && arr[i][j] != '\0'; j++) coe[j] = arr[i][j];
        if(!j) {
            coe[j] = '1';
        }
        if(isNumber(coe)) {
            Nnew->coe = atoi(coe);
        }
        else if(coe[0] == '-') {
            Nnew->coe = -1;
        }
        if(arr[i][j] != '\0') {
            Nnew->var = arr[i][j];
            if (arr[i][j+1] == '^') {
                int k = 0;
                for ( j=j+2; arr[i][j] != '\0'; j++){
                    idx[k] = arr[i][j];
                    k++;
                }
            }
            else {
                idx[0] = '1';
            }
            if(isNumber(idx)) {
                Nnew->idx = atoi(idx);
            }
        }
        else {
            Nnew->var = '0';
        }
        if (!Nhead) {
            Nlast = Nnew;
            Nhead = Nnew;
        }
        else{
            Nlast->next = Nnew;
            Nlast = Nnew;
        }
        if (ERROR) {
            printf("ERROR\n");
        }
        else if(Nnew->var == '0') {
            printf("%d %c\n", Nnew->coe, Nnew->var);
        }
        else {
            printf("%d %c %d\n", Nnew->coe, Nnew->var, Nnew->idx);
        }
    }
    return Nhead;
}