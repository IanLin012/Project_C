#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>

int i, j, k;

void inputData(int **jobGraph) {
    int job, beforeJob;
    char input[2000];
    char *p = input;
    fgets(input, sizeof(input), stdin);
    while(*p != '\0') {
        if(isalpha(*p)) {
            job = *p - 'a';
            p += 2;
        }
        else if(isdigit(*p)) {
            job = strtod(p, &p);
            ++p;
        }
        if(isalpha(*p)) {
            beforeJob = *p - 'a';
            p += 2;
        }
        else if(isdigit(*p)) {
            beforeJob = strtod(p, &p);
            ++p;
        }
        jobGraph[beforeJob][job] = 1;
    }
}

void topological(int **jobGraph, int *flag, int jobNum) {
    int preJob[jobNum];
    int allJob = 0;
    for(i = 0; i < jobNum; ++i) {
        preJob[i] = 0;
    } 
    for(i = 0; i < jobNum; ++i) {
        for(j = 0; j < jobNum; ++j) {
            if(jobGraph[i][j] == 1) {
                ++preJob[j];
            }
        }
    }
    for(i = 0; i < jobNum; ++i) {
        *flag = 0;
        for(j = 0; j < jobNum; ++j) {
            if(preJob[j] == 0) {
                *flag = 1;
                preJob[j] = -1;
                ++allJob;
                for(k = 0; k < jobNum; ++k) { 
                    if(jobGraph[j][k] == 1) {
                    --preJob[k];
                    }
                }
            }
        }
        if(allJob == jobNum) {
            return;
        } 
        if(*flag == 0) {
            return;
        }
    }
}

int main() {
    int jobNum;
    int flag = 0;
    while(scanf("%d", &jobNum) != EOF) {
        getchar();
        if(jobNum == 0) {
            printf("quit\n");
            return 0;
        }
        int **jobGraph = (int**)calloc(jobNum, sizeof(int*));
        for(i = 0; i < jobNum; ++i) {
            jobGraph[i] = (int*)calloc(jobNum, sizeof(int));
        }
        inputData(jobGraph);
        topological(jobGraph, &flag, jobNum);
        if(flag == 0) {
            printf("false\n");
        }
        else {
            printf("true\n");
        }
    }
}