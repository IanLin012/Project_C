#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX_NODES 1005

typedef struct {
    char from;
    char to;
} Edge;

typedef struct {
    int numNodes;
    int numEdges;
    Edge edges[MAX_NODES];
} Graph;

typedef enum {
    WHITE,
    GRAY,
    BLACK
} Color;

Graph graph;

bool hasCycleUtil(int node, Color colors[], Graph* graph) {
    colors[node] = GRAY;
    for(int i = 0; i < graph->numEdges; ++i) {
        if (graph->edges[i].from == graph->edges[node].to) {
            int nextNode = graph->edges[i].to - '0';
            if (graph->edges[i].to >= 'a' && graph->edges[i].to <= 'z') {
                nextNode = graph->edges[i].to - 'a' + 10;
            } else if (graph->edges[i].to >= 'A' && graph->edges[i].to <= 'Z') {
                nextNode = graph->edges[i].to - 'A' + 36;
            }

            if (colors[nextNode] == GRAY) {
                return true;
            } else if (colors[nextNode] == WHITE && hasCycleUtil(nextNode, colors, graph)) {
                return true;
            }
        }
    }
    colors[node] = BLACK;
    return false;
}


bool hasCycle(Graph* graph) {
    Color colors[MAX_NODES] = {WHITE};
    for(int i = 0; i < graph->numNodes; ++i) {
        if (colors[i] == WHITE && hasCycleUtil(i, colors, graph)) {
            return true;
        }
    }

    return false;
}

void solve() {
    int indeg[510], n, m, ansN;
    int x, y;
    int G[510][510] = {0};
    n = graph.numNodes;
    m = graph.numEdges;
    ansN = 0;
    memset(indeg, 0, sizeof(indeg));
    for(int i = 0; i < m; i++) {
        x = graph.edges[i].to - '0';
        y = graph.edges[i].from - '0';
        G[x][y] = 1;
        indeg[y]++;
    }
int fir=0;
    for(int i = 0; i < n; i++) {
        if (indeg[i] == 0) {
            ansN++;
            if(fir==0){
            printf("%d",i);
            fir++;
}
else printf(" %d", i);
            for (int j = 0; j < n; j++) {
                if (G[i][j] == 1) {
                    indeg[j]--;
                }
            }
        }
        if (ansN == n)
            break;
        else if (i == (n - 1)) {
            i = 0;
        }
    }
    printf("\n");
}

int main() {
    int count;
    int job;
    char s[1005];
    while (scanf("%d", &job)) {
        if (job == 0) {
            printf("quit\n");
            break;
        }
        count = 0;
        getchar();
        gets(s);
        for (int i = 0; i < strlen(s); i++) {
            if (i % 4 == 0)
                graph.edges[count].from = s[i];
            if (i % 4 == 2)
                graph.edges[count++].to = s[i];
        }
        graph.numNodes = job;
        graph.numEdges = count;
        bool result = hasCycle(&graph);
        for (int i = 0; i < count; i++) {
            for (int j = 0; j < count; j++) {
                if (graph.edges[i].from == graph.edges[j].to && graph.edges[j].from == graph.edges[i].to)
                    result = true;
            }
        }
        if (result) {
            printf("error\n");
        } else {
            solve();
        }
    }
    return 0;
}