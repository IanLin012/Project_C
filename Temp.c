#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>

#define maxLen 100

int main() {
    char func;
    int arr[maxLen];
    int size = 0;
    while(true) {
        scanf("%c", &func);
        if(func == '0') {
            printf("quit\n");
            break;
        }
        else if(func == 'i') {
            int num, i=0;
            while(true) {
                scanf("%d", &num);
                if(num == '\n') {
                    printf("%d\n", size);
                    break;
                }
                arr[i] = num;
                size += 1;
                i++;
            }
            for(int i=0; i<size; i++) {
                for(int j=0; j<i; j++) {
                    if(arr[j] > arr[i]) {
                        int temp = arr[j];
                        arr[j] = arr[i];
                        arr[i] = temp;
                    }
                }
            }
        }
        else if(func == 'f') {
            int grade;
            scanf("%d", &grade);
            if(grade > size) {
                printf("NULL\n");
            }
            else {
                printf("%d\n", arr[grade-1]);
            }
        }
    }
    return 0;
}

