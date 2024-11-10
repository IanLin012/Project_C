#include <stdio.h>
#include <ctype.h>

typedef struct Polynomial
{
    int coe, exp;
    struct Polynomial *head;
} Polynomial;

void pread();

int main() {
    return 0;
}

void pread() {
    char poly[100];
    char *ptr = poly;
    char alph;

    scanf("%s", poly);
    ptr = poly;

    if(!isalpha(*ptr)) {
        pread();
        return;
    }
    else {
        alph = *ptr;
    }
    ptr++;

    if(*ptr != '=') {
        pread();
        return;
    }
    ptr++;
    
}