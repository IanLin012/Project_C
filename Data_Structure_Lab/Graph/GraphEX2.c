#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int main() {
    int choice,treeNumber,find[1000] = {0};
    char tree[1001][1000] = {0};
    while(scanf("%d",&choice)) {
        if(choice == 0) {
            printf("quit\n");
            break;
        }
        else if(choice == 1) {
            treeNumber=1;
            scanf("%s",&tree[treeNumber]);
            while(tree[treeNumber][0] != 'e') {
                treeNumber++;
                scanf("%s",&tree[treeNumber]);
            }
            printf("OK\n");
        }
        else if(choice == 2) {
            int sum, j=1, add=2, ans[1000]={0};
            if(tree[1][0]==NULL || tree[1][0]=='e') {
                printf("ERROR\n");
            }
            else {
                scanf("%d",&sum);
                for(int count=1 ; count<treeNumber ; count++) {
                    if(count == 1) {
                        find[1]=0;
                    }
                    else if(count == 2) {
                        find[2]=1;
                    }
                    else {
                        if(j == 1) {
                            find[count]=add;
                            j++;
                        }
                        else {
                            find[count]=add;
                            add++;
                            j=1;
                        }
                    }
                }
                int copyTreeNumber=treeNumber-1;
                int x=1,change;
                for(int i=1 ; i<treeNumber ; i++) {
                    while(copyTreeNumber != 1) {
                        change=atoi(tree[copyTreeNumber]);
                        ans[i]=change+ans[i];
                        copyTreeNumber=copyTreeNumber-find[copyTreeNumber];
                    }
                    change=atoi(tree[1]);
                    ans[i]=change+ans[i];
                    copyTreeNumber=treeNumber-1-x;
                    x++;
                }
                for(int i=0 ; i<treeNumber ; i++) {
                    if(sum == ans[i]) {
                        printf("YES\n");
                        break;
                    }
                    if(i == treeNumber-1) {
                        printf("NO\n");
                    }
                }
            }
        }
        else if(choice == 3) {
            int sum2, ans2[100][100]={0,0}, j=1, add=2;
            if(tree[1][0]==NULL || tree[1][0]=='e') {
                printf("ERROR\n");
            }
            else {
                scanf("%d",&sum2);
                for(int count=1 ; count<treeNumber ; count++) {
                    if(count == 1) {
                        find[1]=0;
                    }
                    else if(count == 2) {
                        find[2]=1;
                    }
                    else {
                        if(j == 1) {
                            find[count]=add;
                            j++;
                        }
                        else {
                            find[count]=add;
                            add++;
                            j=1;
                        }
                    }
                }
                int copyTreeNumber2=treeNumber-1;
                int X=1,change2;
                for(int i=1 ; i<treeNumber ; i++) {
                    int J=2;
                    while(copyTreeNumber2 != 1) {
                        change2=atoi(tree[copyTreeNumber2]);
                        ans2[i][J]=change2;
                        J++;
                        ans2[i][1]=change2+ans2[i][1];
                        copyTreeNumber2=copyTreeNumber2-find[copyTreeNumber2];
                    }
                    change2=atoi(tree[1]);
                    ans2[i][1]=change2+ans2[i][1];
                    ans2[i][J]=change2;
                    J++;
                    copyTreeNumber2=treeNumber-1-X;
                    X++;
                }
                int right=0;
                for(int i=treeNumber-1 ; i>0 ; i--) {
                    int count=0;
                    if(sum2 == ans2[i][1]) {
                        for(int k=2; ans2[i][k]!=0 ; k++) {
                            count++;
                        }
                        for(int k=count; k>=2 ; k--){
                            if(k == count){
                                printf("%s+",tree[1]);
                            }
                            printf("%d",ans2[i][k]);
                            if(k != 2) {
                                printf("+");
                            }
                            else {
                                printf("=");
                                printf("%d\n",ans2[i][1]);
                            }
                        }
                        right=1;
                    }
                    else if(i==1 && right==0) {
                        printf("NO\n");
                    }
                }
            }
        }
    }
}