#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 定義二元樹節點結構
typedef struct TreeNode {
    char data[50]; // 假設每個節點最多有50個字符
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

// 創建一個新的樹節點
TreeNode *createNode(const char *data) {
    TreeNode *newNode = (TreeNode *)malloc(sizeof(TreeNode));
    if (newNode != NULL) {
        strncpy(newNode->data, data, sizeof(newNode->data) - 1);
        newNode->data[sizeof(newNode->data) - 1] = '\0'; // 確保字串結尾有終止符
        newNode->left = NULL;
        newNode->right = NULL;
    }
    return newNode;
}

// 比較函數，僅考慮字母
int compareChars(const void *a, const void *b) {
    return (*(char *)a - *(char *)b);
}

// 對字串內的字符進行排序，僅考慮字母
void sortStringChars(char *str) {
    qsort(str, strlen(str), sizeof(char), compareChars);
}

// 插入節點到二元樹，並在插入後直接輸出排序後的字串
TreeNode *insert(TreeNode *root, const char *data) {
    if (root == NULL) {
        // 創建新節點
        TreeNode *newNode = createNode(data);

        // 對字串內的字符進行排序
        sortStringChars(newNode->data);

        // 輸出排序後的字串
        printf("Sorted string: %s\n", newNode->data);

        return newNode;
    }

    int compareResult = strcmp(data, root->data);

    if (compareResult < 0) {
        root->left = insert(root->left, data);
    } else if (compareResult > 0) {
        root->right = insert(root->right, data);
    }

    return root;
}

// 主函數
int main() {
    TreeNode *root = NULL;
    char inputString[50];

    printf("Please enter strings continuously, enter '0' to quit:\n");

    while (1) {
        // 使用者輸入字串
        printf("Enter a string: ");
        scanf("%s", inputString);

        // 檢查是否輸入0
        if (strcmp(inputString, "0") == 0) {
            printf("Quit\n");
            break;
        }

        // 插入節點
        root = insert(root, inputString);
    }

    return 0;
}
