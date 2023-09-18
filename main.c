#include <stdio.h>

struct Date {
    int year;
    int month;
    int day;
};
struct Record {
    char who[10];
    char what[10];
    char where[10];
    struct Date when;
};

void Search() {
    struct Record *record;
    struct Date date;
    char type;
    char key_word[10];
    int y, m, d;
    printf("請輸入要以哪種類型搜尋(如:人、事、時、地): ");
    scanf("%c", &type);
    if(type == '人') {
        printf("請輸入關鍵字: ");
        scanf("%s", &key_word);
        for(int i=0; i<10; i++) {
            if(key_word == record[i].who) {
                printf("%s %s %s %s", record[i].who, record[i].what, record[i].when, record[i].where);
                break;
            }
            else if(i == 9) {
                printf("查無此關鍵字");
        }
        }
    }
    if(type == '事') {
        printf("請輸入關鍵字: ");
        scanf("%s", &key_word);
        for(int i=0; i<10; i++) {
            if(key_word == record[i].what) {
                printf("%s %s %s %s", record[i].who, record[i].what, record[i].when, record[i].where);
                break;
            }
            else if(i == 9) {
                printf("查無此關鍵字");
        }
        }
    }
    if(type == '時') {
        printf("請輸入關鍵字: ");
        scanf("%d/%d/%d", &y, &m, &d);
        for(int i=0; i<10; i++) {
            if(y == record[i].when.year && m == record[i].when.month && d == record[i].when.day) {
                printf("%s %s %s %s", record[i].who, record[i].what, record[i].when, record[i].where);
                break;
            }
            else if(i == 9) {
                printf("查無此關鍵字");
        }
        }
    }
    if(type == '地') {
        printf("請輸入關鍵字: ");
        scanf("%s", &key_word);
        for(int i=0; i<10; i++) {
            if(key_word == record[i].where) {
                printf("%s %s %s %s", record[i].who, record[i].what, record[i].when, record[i].where);
                break;
            }
            else if(i == 9) {
                printf("查無此關鍵字");
        }
        }
    }
    return 0;
}