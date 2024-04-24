#include <stdbool.h>
#include "3_asm_pass1.c"

void fillZero(char* p, int len, int all_len) {
    if(all_len > len) {
        char tmp[1000];
        for(int i=0; i<all_len-len; i++) {
            tmp[i] = '0';
        }
        for(int i=all_len-len, j=0; i<all_len ;i++, j++) {
            tmp[i] = *(p+j);
        }
        tmp[all_len] = '\0';
        strcpy(p, tmp);
    }
};

int Bin_to_Dec(char* str, int len) {
    int sum = 0;
    for(int i=0; i<len; i++) {
        sum *= 2;
        if(*(str+i) == '1') {
            sum += 1;
        }
    }
    return sum;
}

int transBin(char* seg, int n) {
    char to2[] = "01";
    int i, a, b;
    for(i=0; n>0; i++) {
        *(seg+i) = to2[n%2];
        n /= 2;
    }
    seg[i] = '\0';
    // reverse
    for(a=0, b=i-1; a<b; a++, b--) {
        char tmp = seg[a];
        seg[a] = seg[b];
        seg[b] = tmp;
    }
    return i;
}

int transHex(char* seg, int n) {
    char to16[] = "0123456789ABCDEF";
    int i, a, b;
    for(i=0; n>0; i++) {
        *(seg+i) = to16[n%16];
        n /= 16;
    }
    seg[i] = '\0';
    // reverse
    for(a=0, b=i-1; a<b; a++, b--) {
        char tmp = seg[a];
        seg[a] = seg[b];
        seg[b] = tmp;
    }
    return i;
}

void cntAddr(LINE line, int* addSum) {
    if(!(line.code<=OP_END && line.code>=OP_BASE)) {
        if(line.code<=OP_RESW && line.code>=OP_BYTE) {
            if(line.code == OP_BYTE) {
                if(line.operand1[0] == 'X') {
                    *addSum += 1;
                }
                else if(line.operand1[0] == 'C') {
                    *addSum += strlen(line.operand1)-3;
                }
            }
            else if(line.code == OP_WORD) {
                *addSum += 3;
            }
            else if(line.code == OP_RESB) {
                int cnt = 0;
                for(int i=0; i<strlen(line.operand1); i++) {
                    cnt *= 10;
                    cnt += line.operand1[i]-'0';
                }
                *addSum += cnt;
            }
            else if(line.code == OP_RESW) {
                int cnt = 0;
                for(int i=0; i<strlen(line.operand1); i++) {
                    cnt *= 10;
                    cnt += line.operand1[i]-'0';
                }
                *addSum += cnt*3;
            }
        }
        else {
            if(line.fmt == FMT1) {
                *addSum += 1;
            }
            else if(line.fmt == FMT2) {
                *addSum += 2;
            }
            else if(line.fmt == FMT3) {
                *addSum += 3;
            }
            else if(line.fmt == FMT4) {
                *addSum += 4;
            }
        }
    }
}

char sym[1000][100], sym_line[1000][100], objcode[1000][100];
int sym_address[1000], sym_size=0;

void trans_objCode_STD(LINE line, char* objCode) {
    int objCodelen;
    if(!(line.code<=OP_END && line.code>=OP_BASE)) {
        if(line.code<=OP_RESW && line.code>=OP_BYTE) {
            if(line.code == OP_BYTE) {
                if(line.operand1[0] == 'X') {
                    int i;
                    for(i=2; line.operand1[i]!='\''; i++) {
                        *(objCode+i-2) = line.operand1[i];
                    }
                    *(objCode+i-2) = '\0';
                }
                else if(line.operand1[0] == 'C') {
                    int i;
                    for(i=2; line.operand1[i]!='\''; i++) {
                        transHex(objCode+(i-2)*2, line.operand1[i]);
                    }
                    *(objCode+(i-2)*2) = '\0';
                }
            }
            else if(line.code == OP_WORD) {
                int sum = 0;
                for(int i=0; i<strlen(line.operand1); i++) {
                    sum *= 10;
                    sum += line.operand1[i]-'0';
                }
                objCodelen = transHex(objCode, sum);
                fillZero(objCode, objCodelen, 6);
            }
            else if(line.code == OP_RESB) {
            }
            else if(line.code == OP_RESW) {
            }
        }
        else {
            char tmp[1000];
            objCodelen = transBin(objCode, (int)line.code);
            fillZero(objCode, objCodelen, 8);
            for(int i=0; i<sym_size; i++) {
                if(strcmp(line.operand1, sym[i]) == 0) {
                    memset(tmp, 0, sizeof(tmp));
                    transBin(tmp, sym_address[i]);
                    fillZero(tmp, strlen(tmp), 15);
                    for(int j=9; j<9+15; j++) {
                        *(objCode + j) = tmp[j - 9];
                    }
                    *(objCode+24) = '\0';
                    break;
                }
            }
            if((line.addressing & 0x8) == 0x8) {
                *(objCode+8) = '1';
            }
            else {
                *(objCode+8) = '0';
            }
            memset(tmp, 0, sizeof(tmp));
            objCodelen = transHex(tmp, Bin_to_Dec(objCode, 24));
            strcpy(objCode, tmp);
            fillZero(objCode, objCodelen, 6);
        }
    }
}

int char_to_int(char *str, int len) {
    int sum = 0;
    for(int i=0; i<len; i++) {
        sum *= 10;
        sum += *(str+i)-'0';
    }
    return sum;
}

int main(int argc, char* argv[]) {
    int c, line_count, addSum=0, beginAddr=0, lineSize=0, lineAddr[1000];
    char add_16bit_cnt[7], tmpFmt[7], tmpAddr[7];
    LINE line[1000];
    if(argc < 2) {
        printf("Usage: %s fname.asm\n", argv[0]);
    }
    else {
        if(ASM_open(argv[1]) == NULL) {
            printf("File not found!!\n");
        }
        else {
            // pass 1
            for(line_count=1; (c=process_line(&line[lineSize]))!=LINE_EOF; line_count++) {
                if(c == LINE_ERROR) {
                    printf("%03d : Error\n", line_count);
                }
                else if(c == LINE_COMMENT) {
                }
                else {
                    memset(tmpAddr, 0, sizeof(tmpAddr));
                    memset(tmpFmt, 0, sizeof(tmpFmt));
                    memset(objcode[lineSize], 0, sizeof(objcode[lineSize]));
                    if(strcmp(line[lineSize].op, "START") == 0) {
                        int cnt = 0;
                        for(int i=0; i<strlen(line[lineSize].operand1); i++) {
                            cnt *= 10;
                            cnt += line[lineSize].operand1[i] - '0';
                        }
                        addSum = cnt;
                        beginAddr = cnt;
                        transHex(add_16bit_cnt, addSum);
                    }
                    transBin(tmpFmt, (int)line[lineSize].fmt);
                    transBin(tmpAddr, (int)line[lineSize].addressing);
                    if(line[lineSize].symbol[0] != '\0' && strcmp(line[lineSize].op, "START") != 0) {
                        strcpy(sym[sym_size], line[lineSize].symbol);
                        strcpy(sym_line[sym_size], add_16bit_cnt);
                        sym_address[sym_size] = addSum;
                        sym_size++;
                    }
                    lineAddr[lineSize] = addSum;
                    cntAddr(line[lineSize], &addSum);
                    transHex(add_16bit_cnt, addSum);
                    lineSize++;
                }
            }
            transHex(add_16bit_cnt, addSum-beginAddr);
            for(int i=0; i<sym_size; i++) {
            }
            // pass 2
            for(int i=0; i<lineSize; i++) {
                memset(tmpAddr, 0, sizeof(tmpAddr));
                memset(tmpFmt, 0, sizeof(tmpFmt));
                memset(objcode[i], 0, sizeof(objcode[i]));
                transHex(add_16bit_cnt, lineAddr[i]);
                transBin(tmpFmt, (int)line[i].fmt);
                transBin(tmpAddr, (int)line[i].addressing);
                trans_objCode_STD(line[i], objcode[i]);
            }
            // print pass2
            transHex(tmpAddr, beginAddr);
            transHex(add_16bit_cnt, addSum - beginAddr);
            if(strcmp(line[0].op, "START")==0) {
                printf("H%-6s%06s%06s\n", line[0].symbol, tmpAddr, add_16bit_cnt);
            }
            else{
                printf("H%-6s%06s%06s\n", " ", tmpAddr, add_16bit_cnt);
            }
            int line_cnt=0, add_cnt=beginAddr, text_max=0;
            bool mem = 0;
            while(line_cnt<lineSize){
                mem = 0;
                memset(tmpAddr, 0, sizeof(tmpAddr));
                transHex(tmpAddr, add_cnt);
                printf("T%06s ", tmpAddr);
                int pre = line_cnt;
                while(line_cnt < lineSize){
                    if(line[line_cnt].code==OP_RESB || line[line_cnt].code==OP_RESW) {
                        break;
                    }
                    if(text_max+strlen(objcode[line_cnt])<=60) {
                        text_max+=strlen(objcode[line_cnt]);
                        line_cnt++;
                    }
                    else {
                        break;
                    }
                }
                transHex(tmpAddr, text_max/2);
                printf("%02s", tmpAddr);
                line_cnt = pre;
                text_max = 0;
                while(line_cnt < lineSize) {
                    if(line[line_cnt].code==OP_RESB || line[line_cnt].code==OP_RESW) {
                        mem = 1;
                        if(line[line_cnt].code == OP_RESW){
                            add_cnt+=3*char_to_int(line[line_cnt].operand1, strlen(line[line_cnt].operand1));
                        }
                        else if(line[line_cnt].code == OP_RESB){
                            add_cnt+=char_to_int(line[line_cnt].operand1, strlen(line[line_cnt].operand1));
                        }
                        line_cnt++;
                        continue;
                    }
                    else if(mem == 1){
                        break;
                    }
                    if(text_max+strlen(objcode[line_cnt]) <= 60) {
                        printf("%s", objcode[line_cnt]);
                        text_max+=strlen(objcode[line_cnt]);
                        line_cnt++;
                    }
                    else {
                        break;
                    }
                }
                printf("\n");
                add_cnt += text_max/2;
                text_max = 0;
            }
            transHex(tmpAddr, beginAddr);
            printf("E%06s", tmpAddr);
            ASM_close();
        }
    }
}