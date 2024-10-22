#include <stdio.h>

// variable declare
long long arr[26][1000][1000], col[26]={0}, row[26]={0}, ct_read=0;
char alph[26] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};

// subroutine declare
void readSparseMatrixs();
void printSparseMatrix();
void addSparseMatrix();
void fastTranspose();
void matrixMultiply();

// main
int main() {
    int func;
    while(scanf("%d", &func) != EOF) {
        getchar();
        if(func == 1) readSparseMatrixs();
        else if(func == 2) printSparseMatrix();
        else if(func == 3) addSparseMatrix();
        else if(func == 4) fastTranspose();
        else if(func == 5) matrixMultiply();
    }
    return 0;
}

// read
void readSparseMatrixs() {
    ct_read+=1;
    int c, r, b=0, ct_0, t_alph=0;
    char al;
    scanf("%c", &al);
    scanf("%d %d", &c, &r);
    while(c!=0 && r!=0 && b == 0) {
        ct_0 = 0;
        for(int k=0; k<26; k++) {
            if(alph[k] == al) {
                arr[k][c][r];
                col[k] = c;
                row[k] = r;
                t_alph = k;
                for(int i=0; i<col[k]; i++) {
                    for(int j=0; j<row[k]; j++) {
                        scanf("%d", &arr[k][i][j]);
                        if(arr[k][i][j] == 0) ct_0+=1;
                    }
                }
            }
        }
        if(ct_0 <= (col[t_alph]*row[t_alph])/2)
            printf("Error. Please try again.\n");
        else if(ct_0 > (col[t_alph]*row[t_alph])/2) {
            printf("Is a Sparse Matrix.\n");
            b = 1;
        }
    }
}

// print (2 \n alph)
void printSparseMatrix() {
    int ct_print = 0;
    for(int k=0; k<26; k++) {
        if(col[k]>0 && row[k]>0) {
            ct_print+=1;
            printf("%c:\n", alph[k]);
            for(int i=0; i<col[k]; i++) {
                for(int j=0; j<row[k]; j++)
                    printf("%d ", arr[k][i][j]);
                printf("\n");
            }
            if(ct_read != ct_print) {
                printf("\n");
            }
        }
    }
}

// add (3 \n alph1 alph2)
void addSparseMatrix() {
    long long add = 0;
    char al1, al2;
    scanf("%c %c", &al1, &al2);
    for(int k=0; k<26; k++) {
        for(int l=0; l<26; l++) {
            if(alph[k] == al1 && alph[l] == al2) {
                for(int i=0; i<col[k]; i++) {
                    for(int j=0; j<row[k]; j++) {
                        add = arr[k][i][j] + arr[l][i][j];
                        printf("%d ", add);
                    }
                    printf("\n");
                }
            }
        }
    }
}

// transpose (4 \n alph)
void fastTranspose() {
    char al;
    scanf("%c", &al);
    for(int k=0; k<26; k++) {
        if(alph[k] == al) {
            for(int i=0; i<row[k]; i++) {
                for(int j=0; j<col[k]; j++)
                    printf("%d ", arr[k][j][i]);
                printf("\n");
            }
        }
    }
}

// multiply (5 \n alph1 alph2)
void matrixMultiply() {
    long long mul[1000][1000];
    char al1, al2;
    scanf("%c %c", &al1, &al2);
    for(int k=0; k<26; k++) {
        for(int l=0; l<26; l++) {
            if(alph[k]==al1 && alph[l]==al2) {
                for(int i=0; i<col[k]; i++) {
                    for(int j=0; j<row[l]; j++) {
                        for(int inner=0; inner<row[k]; inner++)
                            mul[i][j] += arr[k][i][inner] * arr[l][inner][j];
                        printf("%d ", mul[i][j]);
                    }
                    printf("\n");
                }
            }
        }
    }
}