#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_SYM 100

typedef struct Table {
    char key[10];
    int value, type;
    int secLen;
} Table;

int _establen_ = 0;
Table estab[MAX_SYM];

void pushTable(char *key, int value, int type) {
    if (_establen_ >= MAX_SYM) return;
    strcpy(estab[_establen_].key, key);
    estab[_establen_].value = value;
    estab[_establen_].type = type;
    _establen_++;
}

void addCSLenToTable(char *key, int secLen) {
    for (int i=0; i<_establen_; i++) {
        if (estab[i].type == 1 && !strcmp(estab[i].key, key)) {
            estab[i].secLen = secLen;
        }
    }
}

int findTable(char *key) {
    for (int i=0; i<_establen_; i++) {
        if (!strcmp(estab[i].key, key)) {
            return estab[i].value;
        }
    }
    return -1;
}

void printTable() {
    printf("List of ESTAB:\n");
    printf("%33s\n", "---------------------------------");
    printf("%-6s | %-6s | %-6s | %-6s\n", "CS", "SYMBOL", "ADDR", "CS_LEN");
    printf("%33s\n", "---------------------------------");
    for (int i=0; i<_establen_; i++) {
        if (estab[i].type == 1)
            printf("%-6s | %-6s | %06X | %06X\n", estab[i].key, "", estab[i].value, estab[i].secLen);
        if (estab[i].type == 2)
            printf("%-6s | %-6s | %06X | %6s \n", "", estab[i].key, estab[i].value, "x");
    }
    printf("%33s\n", "---------------------------------");
}

int main(int argc, char *argv[]) {
    FILE *file;
    char line[128], csName[10], subStr[10], symName[10];
    int curfile, csAddr, csOffset, csLen, lineCnt, symAddr;
    if (argc < 2) {
        printf("Usage: loader_pass1.exe <address> <obj_file0> <obj_file1>...");
        return 0;
    }
    for (int i=0; i<strlen(argv[1]); i++) {
        char c=toupper(argv[1][i]);
        if (!isdigit(c) && !(c >= 'A' && c <= 'F')) {
            printf("Use Hex number as address: asm_loader.exe <address> <file 1> <file 2>...");
            return 0;
        }
    }
    sscanf(argv[1], "%6x", &csAddr);
    printf("Program Address Starts at %X\n", csAddr);
    curfile = 1;
    while (++curfile < argc) {
        file = fopen(argv[curfile], "r");
        if (file == NULL) {
            printf("Unable to read file: %s\n", argv[curfile]);
            continue;
        }
        lineCnt=0;
        while (fgets(line, sizeof(line), file)) {
            lineCnt++;
            if (lineCnt == 1) {
                if (line[0] == 'H') {
                    strncpy(subStr, &line[1], 6);
                    subStr[6] = '\0';
                    sscanf(subStr, "%s", csName);
                    strncpy(subStr, &line[1+6], 6);
                    subStr[6] = '\0';
                    sscanf(subStr, "%x", &csOffset);
                    strncpy(subStr, &line[1+6+6], 6);
                    subStr[6] = '\0';
                    sscanf(subStr, "%x", &csLen);
                } else {
                    printf("Unable to determine Header record, skipping file: %s\n", argv[curfile]);
                    break;
                }
                if (findTable(csName) != -1) {
                    printf("Find duplicate external symbol, skipping file: %s\n", argv[curfile]);
                    break;
                } else {
                    csAddr += csOffset;
                    pushTable(csName, csAddr, 1);
                    addCSLenToTable(csName, csLen);
                }
            } else if (line[0] != 'E') {
                if (line[0] == 'D') {
                    for (int i=0; line[1+12*i] != '\n'; i++) {
                        strncpy(subStr, &line[1+12*i], 6);
                        subStr[6] = '\0';
                        sscanf(subStr, "%s", symName);
                        strncpy(subStr, &line[7+12*i], 6);
                        subStr[6] = '\0';
                        sscanf(subStr, "%x", &symAddr);
                        if (findTable(symName) != -1) {
                            printf("Find duplicate external symbol, skipping symbol: %s\n", symName);
                            continue;
                        } else {
                            pushTable(symName, csAddr+symAddr, 2);
                        }
                    }
                }
                if (line[0] == 'R') {/*do nothing*/};
            }
            else if (line[0] == 'E') {
                continue;
            }
        }
        csAddr += csLen;
    }
    printTable();
    fclose(file);
    return 0;
}