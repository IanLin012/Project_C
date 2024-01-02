#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define maxKey 100
#define maxData 21

struct element {
    int index;
    int keys[maxKey];
    char who[maxKey][maxData], what[maxKey][maxData], when[maxKey][maxData], where[maxKey][maxData];
} ele;

void checkKey(struct element *ele, int key) {
    for(int i=0; i<ele->index; i++) {
        if(ele->keys[i] == key) {
            printf("KEY EXIST:");
            for(int j=0; j<ele->index; j++) {
                printf(" %d", ele->keys[j]);
            }

            printf("\nNew key:\n");
            scanf("%d", &key);
            ele->keys[ele->index] = key;
            break;
        }
    }
}

void input(struct element *ele) {
    scanf("%d", &ele->keys[ele->index]);
    getchar();

    char tempWho[maxKey], tempWhat[maxKey], tempWhen[maxKey], tempWhere[maxKey];
    fgets(tempWho, sizeof(tempWho), stdin);
    fgets(tempWhat, sizeof(tempWhat), stdin);
    fgets(tempWhen, sizeof(tempWhen), stdin);
    fgets(tempWhere, sizeof(tempWhere), stdin);

    tempWho[strlen(tempWho) - 1] = '\0';
    tempWhat[strlen(tempWhat) - 1] = '\0';
    tempWhen[strlen(tempWhen) - 1] = '\0';
    tempWhere[strlen(tempWhere) - 1] = '\0';

    checkKey(ele, ele->keys[ele->index]);

    strcpy(ele->who[ele->index], tempWho);
    strcpy(ele->what[ele->index], tempWhat);
    strcpy(ele->when[ele->index], tempWhen);
    strcpy(ele->where[ele->index], tempWhere);

    printf("OK\n");
    ele->index++;
}

void print(struct element *ele) {
    int item = 0;
    for(int i=0; i<ele->index; i++) {
        if (ele->keys[i] != 0) {
            printf("%d\n%s, %s, %s, %s\n", ele->keys[i], ele->who[i], ele->what[i], ele->when[i], ele->where[i]);
            item++;
        }
    }
    if(item == 0) {
        printf("No Data\n");
    }
}

int compareKeys(const void *a, const void *b) {
    return(*(int *)b - *(int *)a);
}

void createHeap(struct element *ele) {
    qsort(ele->keys, ele->index, sizeof(int), compareKeys);

    for(int i=0; i<ele->index; i++) {
        if(ele->keys[i] != 0) {
            if(i == ele->index-1) {
                printf("%d\n", ele->keys[i]);
                break;
            }
            else {
                printf("%d ", ele->keys[i]);
            }
        }
    }

    int item = 0;
    for(int i=0; i<ele->index; i++) {
        if(ele->keys[i] != 0) {
            printf("%d\n%s, %s, %s, %s\n", ele->keys[i], ele->who[i], ele->what[i], ele->when[i], ele->where[i]);
            item++;
        }
        ele->keys[i] = 0;
    }
    if(item == 0) {
        printf("No Data\n");
    }
}

int main() {
    int func;
    ele.index = 0;
    while(scanf("%d", &func) != EOF) {
        if(func == 0) {
            printf("quit\n");
            break;
        }
        else if((func < 0) || (func > 3)) {
            printf("ERROR\n");
        }
        else if(func == 1) {
            input(&ele);
        }
        else if(func == 2) {
            print(&ele);
        }
        else if(func == 3) {
            createHeap(&ele);
            ele.index = 0;
        }
    }
    return 0;
}