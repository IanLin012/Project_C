#include <stdio.h>

#define maxSize 100

int *findGrade(int[], int, int);

int main() {
    int arr[maxSize], size=0;
    char func;
    while(1) {
        scanf(" %c", &func);
        if (func == '0') {
            printf("quit");
            break;
        }
        else if(func == 'i') {
            size = 0;
            while(1) {
                int num;
                if (scanf("%d", &num) == 1) {
                    arr[size++] = num;
                }
                else {
                    break;
                }
            }

        }
        else if (func == 'f') {
            int grade;
            scanf("%d",&grade);
            int* result = findGrade(arr, size, grade);
            if(result != NULL) {
                printf("%d\n", *result);
            }
            else {
                printf("NULL\n");
            }
        }
    }
    return 0;
}

int *findGrade(int arr[], int n, int i) {
    if(i>0 && i<=n) {
        for(int j=0; j<n-1; j++) {
            for(int k=0; k<n-j-1; k++) {
                if(arr[k] > arr[k+1]) {
                    int temp = arr[k];
                    arr[k] = arr[k + 1];
                    arr[k + 1] = temp;
                }
            }
        }
        return(&arr[i-1]);
    }
    else {
        return NULL;
    }
}