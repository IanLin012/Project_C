#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <math.h>
#include <stdbool.h>
#include <ctype.h>

typedef struct func{
    int coe;
    char var;
    int index;
    struct func *next;
}func;


typedef struct equPointers{
    int n;
    char code;
    struct func *first;
    struct equPointers *next;
}equPointers;

bool isNumber(const char *str) {
    if (str == NULL || *str == '\0') return false;
    if (*str == '+' || *str == '-') str++;
    bool hasDot = false;
    bool hasDigit = false;
    while (*str != '\0') {
        if (*str >= '0' && *str <= '9') {
            hasDigit = true;
        } else if (*str == '.') {
            if (hasDot) return false;
            hasDot = true;
        } else {
            return false;
        }
        str++;
    }
    return hasDigit;
}



char save[10][10];
int segmentation(char * poly){
    int j = 0, k;
    for (int i = 0; i < 10; i++){
        for (int j = 0; j < 10; j++){
            save[i][j] = '\0';
        }
    }
    for (int i = 0; poly[i] != '\0'; i++){
        k = 0;
        if(poly[i] == '-'){
            save[j][k] = '-';
            k++;
            i++;
        }
        for (; poly[i] != '*' && poly[i] != '\0'; i++){
            save[j][k] = poly[i];
            k++;
        }
        if (poly[i+1] == '-') i++;
        j++;
    }
    return j;
}

bool debug(){
    for (int i = 0; save[i][0] != '\0'; i++){
        int k = 0;
        for (int j = 0; save[i][j] != '\0'; j++)
            if (save[i][j] == 'x' || save[i][j] == 'y' || save[i][j] == 'z') k++;
        if (k > 1) return true;
    }
    return false;
}

func * anapoly(bool error){
    func * Nhead = 0, * Nlast = 0;
    for (int i = 0; save[i][0] != '\0'; i++){
        func * Nnew = malloc(sizeof(func));
        Nnew -> next = 0;
        char coe[100] = {'\0'}, index[100] = {'\0'};
        int j = 0;
        for (; save[i][j] != 'x' && save[i][j] != 'y' && save[i][j] != 'z' && save[i][j] != '\0'; j++) coe[j] = save[i][j];
        if(!j) coe[j] = '1';
        if(isNumber(coe)) Nnew -> coe = atoi(coe);
        else if(coe[0] == '-') Nnew -> coe = -1;
        if(save[i][j] != '\0'){
            Nnew -> var = save[i][j];
            if (save[i][j+1] == '^'){
                int k = 0;
                for ( j=j+2; save[i][j] != '\0'; j++){
                    index[k] = save[i][j];
                    k++;
                }
            }else{
                index[0] = '1';
            }
            if(isNumber(index)) Nnew -> index = atoi(index);
        }else
            Nnew -> var = '0';
        if (!Nhead){
            Nlast = Nnew;
            Nhead = Nnew;
        }else{
            Nlast -> next = Nnew;
            Nlast = Nnew;
        }
        if (error) printf("ERROR\n");
        else if(Nnew -> var == '0') printf("%d %c\n", Nnew -> coe, Nnew -> var);
        else printf("%d %c %d\n", Nnew -> coe, Nnew -> var, Nnew -> index);
    }
    return Nhead;
}

equPointers * Phead = 0, * Plast = 0;

int main(){
    while (1){
        bool error = false;
        char poly[256] = {"\0"};
        scanf("%s", poly);
        if (poly[0] == '0') break;
        equPointers * new = malloc(sizeof(equPointers));
        new -> next = 0;
        new -> n = segmentation(poly);
        error = debug();
        new -> first = anapoly(error);
        if (!Phead){
            Plast = new;
            Phead = new;
        }else{
            Plast -> next = new;
            Plast = new;
        }
    }
    printf("quit\n");
    return 0;
}