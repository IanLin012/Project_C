#include<stdio.h>
#include<limits.h>
#include<string.h>

#define V 9

typedef struct Graph {
    int dist[V];        
    int sptSet[V];  
    int stop[V];  
    int graph[V][V];
} Graph;
Graph record;

int minDistance() {
    int min = INT_MAX;
    int min_index;
    for(int v = 0; v < V; ++v) {
        if(record.sptSet[v] == 0 && record.dist[v] <= min) {
            min = record.dist[v];
            min_index = v;
        }
    }
    return min_index;
}

void inputData() {
    int vertex, edge, from, go, price;
    scanf("%d %d", &vertex, &edge);
    memset(record.graph, 0, sizeof(record.graph));
    for(int i = 0; i < edge; ++i) {
        scanf("%d %d %d", &from, &go, &price);
        record.graph[go][from] = price;
    }
    
    printf("OK\n");
}

void printResult(int from, int stop) {
    if(record.stop[from] > stop || record.dist[from] == INT_MAX) {
        printf("NO\n");
        return;
    }
    printf("%d\n", record.dist[from]);
}


void dijkstra(int source) {
    for(int i = 0; i < V; ++i) {
        record.dist[i] = INT_MAX;
        record.sptSet[i] = 0;
        record.stop[i] = -1;
    }
    record.dist[source] = 0;
    for(int count = 0; count < V - 1; ++count) {
        int u = minDistance();
        record.sptSet[u] = 1;
        for(int v = 0; v < V; ++v) {
            if(!record.sptSet[v] && record.graph[u][v] && record.dist[u] != INT_MAX&& record.dist[u] + record.graph[u][v] < record.dist[v]) {
                record.dist[v] = record.dist[u] + record.graph[u][v];
                record.stop[v] = record.stop[u] + 1;
            }
        }
    }    
}

int main() {
    int choice, from, go, stop;
    while(scanf("%d", &choice) != EOF) {
        switch(choice) {
            case 1:
                inputData();
                break;
            case 2:
                scanf("%d %d %d", &from, &go, &stop);
                dijkstra(go);
                printResult(from, stop);
                break;
            case 0:
                printf("quit\n");
                return 0;
        }
    }
}
