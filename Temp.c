#include <stdio.h>
#include <string.h>

void customSort(char* str) {
    int len = strlen(str);
    
    // 將'^'和'v'移到最前面
    for (int i = 0; i < len - 1; i++) {
        for (int j = 0; j < len - i - 1; j++) {
            if ((str[j] == 'v' || str[j] == '^') && (str[j + 1] != 'v' && str[j + 1] != '^')) {
                char temp = str[j];
                str[j] = str[j + 1];
                str[j + 1] = temp;
            }
        }
    }

    // 反轉其餘字母
    int start = 0;
    int end = len - 1;
    while (start < end) {
        if (str[start] != 'v' && str[start] != '^' && str[end] != 'v' && str[end] != '^') {
            char temp = str[start];
            str[start] = str[end];
            str[end] = temp;
            start++;
            end--;
        }
        else {
            if ((str[start] == 'v' || str[start] == '^') || (str[end] == 'v' || str[end] == '^')) {
                start++;
                end--;
            }
            else if (str[start] == 'v' || str[start] == '^') {
                start++;
            }
            else if (str[end] == 'v' || str[end] == '^') {
                end--;
            }
        }
    }
}

int main() {
    char input[100];

    // 使用者輸入字串
    scanf("%s", input);

    // 呼叫副程式進行排序
    customSort(input);

    // 輸出排序後的結果
    printf("%s\n", input);

    return 0;
}
