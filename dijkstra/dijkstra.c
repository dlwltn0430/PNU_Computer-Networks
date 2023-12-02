#include <stdio.h>
#define MAX_NODES 10
#define INF 99999999
#define TRUE 1
#define FALSE 0

typedef struct GraphType {
	int n;
	int cost[MAX_NODES][MAX_NODES];
} GraphType;

int distance[MAX_NODES];
int found[MAX_NODES];

void print_status(GraphType* g, int step) {
    printf("STEP %d ", step);
    printf("\n");
	printf("        distance: ");

    for (int i = 0; i < g->n; i++) {
        if (distance[i] == INF) {
            printf("* ");
        } else {
            printf("%d ", distance[i]);
        }
    }
    printf("\n");
	printf("        found: ");

    for (int i = 0; i < g->n; i++) {
            printf("%d ", found[i]);
    }

    printf("\n");
}

int choose(int distance[], int n, int found[]) {
    int min, min_index;
    min = INF;
    min_index = -1;

    for (int i = 0; i < n; i++) {
        if (distance[i] < min && !found[i]) {
            min = distance[i];
            min_index = i;
        }
    }

    return min_index;
}

void shortest_path(GraphType* g, int start_node) {
    for (int i = 0; i < g->n; i++) {
        distance[i] = g->cost[start_node][i];
        found[i] = FALSE;
    }

    found[start_node] = TRUE;
    distance[start_node] = 0;

    int u;
    for (int i = 0; i < g->n - 1; i++) {
        print_status(g, i);
        u = choose(distance, g->n, found);
        found[u] = TRUE;
        for (int z = 0; z < g->n ; z++) {
            if (!found[z]) {
                if (distance[u] + g->cost[u][z] < distance[z]) {
                    distance[z] = distance[u] + g->cost[u][z];
                }
            }
        }
    }

}

int main() {
    int start_node_index = 1;

	GraphType G = {6,
	{{0, 5, 3, 7, INF, INF},
	{5, 0, 4, INF, 7, 9},
	{3, 4, 0, 3, 8, INF},
	{7, INF, 3, 0, 4, INF},
	{INF, 7, 8, 4, 0, 2},
	{INF, 9, INF, INF, 2, 0}}
	};

    shortest_path(&G, start_node_index);

	return 0;
}
