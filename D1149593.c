#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <math.h>

struct StockData {
    int date[200];
    float price[200];
    int total[200];
    float rate[200];
};

struct StockData sd;
int count = 0;
float max, min;

void findMinMaxPrice() {
    max = sd.price[0];
    min = sd.price[0];
    for (int i=1; i<170; i++) {
        if (sd.price[i] > max){
            max = sd.price[i];
        }
        if(sd.price[i] < min){
            min = sd.price[i];
        }
    }
}

int maxk, mink;
void findMinMaxTotalShareholders() {
    max = sd.rate[0];
    min = sd.rate[0];
    for (int i=0; i<170; i++) {
        if (sd.rate[i] > max){
            max = sd.rate[i];
            maxk = i;
        }
        if(sd.rate[i] < min) {
            min = sd.rate[i];
            mink = i;
        }
    }
}

float value;

void findDatesByPrice() {
    count = 0;
    for (int i = 0; i < 170; i++) {
        if (sd.price[i] == value) {
            printf("%d ", sd.date[i]);
            count = 1;
        }
    }
    if (count == 0) {
        printf("not exist");
    }
    printf("\n");
}

float week1, week4;

void calculateChanges() {
    week1 = sd.rate[0]-sd.rate[1];
    week4 = sd.rate[0]-sd.rate[4];
}

int main() {

    FILE *file= fopen("1102.TW.csv", "r");
    if (file == NULL) {
        perror("error to open the file");
        return 1;
    }
    char line[1024];
    count = 0;
    int l = 0;
    while (fgets(line, sizeof(line), file)) {
        if(l == 1) {
            char data[13][20] = { };
            int k = 0;
            int j[13];
            for (int i = 0; i < 13; i++) {
                j[i] = 0;
                if (line[k] == '"') {
                    k++;
                    while(line[k] != '"') {
                        if (line[k] != ',') {
                            data[i][j[i]] = line[k];
                            j[i]++; k++;
                        }
                        else {
                            k++;
                        }
                    }
                    k++;
                }
                else {
                    while (line[k] != ',' && line[k] != '\n') {
                        data[i][j[i]] = line[k];
                        j[i]++; k++;
                    }
                }
                k++;
            }
            sd.date[count] = atoi(data[0]);
            sd.rate[count] = atof(data[2]);
            sd.price[count] = atof(data[3]);
            sd.total[count] = atoi(data[5]);
            count++;
        }
        l = 1;
    }
    fclose(file);
    
    findMinMaxPrice();
    printf("程基: %.2f\n", min);
    printf("程蔼基: %.2f\n", max);

    findMinMaxTotalShareholders();
    printf("癸莱程Μ絃基: %.2f 计: %d\n", sd.price[mink], sd.total[mink]);
    printf("癸莱程蔼Μ絃基: %.2f 计: %d\n", sd.price[maxk], sd.total[maxk]);
  
    printf("叫块ヘ夹Μ絃基: ");
    scanf("%f", &value);
    findDatesByPrice();

    calculateChanges();
    printf("程秅狥秈ゑ瞯: %.2f\n", week1);
    printf("程秅狥秈ゑ瞯: %.2f\n", week4);

    return 0;
}