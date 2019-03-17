#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
// Максимальное количество вершин
#define MAX_VERTICES 5000  

typedef struct edge {
	int start;
	int finish;
	int weight;
} Edge;

typedef struct ansEdge {
	int start;
	int finish;
} AnsEdge;

void checkAndRead(FILE *in, int numOfVertices, int numOfEdges, Edge *edge) {

	if (numOfVertices < 0 || numOfVertices > MAX_VERTICES) {
		printf("bad number of vertices");
		free(edge);
		exit(0);
	}

	if (numOfEdges < 0 || numOfEdges >(numOfVertices*(numOfVertices - 1) / 2)) {
		printf("bad number of edges");
		free(edge);
		exit(0);
	}
	if (((numOfVertices == 0) || (numOfVertices > 1)) && (numOfEdges == 0)){
		printf("no spanning tree");
		free(edge);
		exit(0);
	}
	if (numOfEdges < numOfVertices - 1){
		printf("no spanning tree");
		free(edge);
		exit(0);
	}
	long c = 0;
	for (int i = 0; i < numOfEdges; i++) {
		if (fscanf(in, "%d %d %ld", &edge[i].start, &edge[i].finish, &c) != EOF) {
			edge[i].start = edge[i].start-1;
			edge[i].finish = edge[i].finish-1;		
		}
		else {
			printf("bad number of lines");
			free(edge);
			exit(0);
		}
		if (c < 0 || c > INT_MAX) {
			printf("bad length");
			free(edge);
			exit(0);
		}
		edge[i].weight = c;
		if (edge[i].start < 0 || edge[i].start > numOfVertices - 1 || edge[i].finish < 0 || edge[i].finish > numOfVertices - 1) {
			printf("bad vertex");
			free(edge);
			exit(0);
		}
	}
}

void prim( Edge *edge, int numOfVertices, int numOfEdges, AnsEdge *ansEdge) {
	if (numOfEdges > MAX_VERTICES){
		for (int i = 0; i < numOfVertices - 1; i++)
			printf("%d %d\n", i + 1, i + 2);
	}
	else {
		int *visited = (int*)malloc(sizeof(int) * numOfVertices);
		int *selectedEdges = (int*)malloc(sizeof(int) * numOfVertices);
		unsigned int *minEdges = (int*)malloc(sizeof(unsigned int) * numOfVertices);

		for (int i = 0; i < numOfVertices; i++) {
			minEdges[i] = INT_MAX;
			minEdges[i]++;
			selectedEdges[i] = -1;
			visited[i] = 0;
		}
		minEdges[0] = 0;
		int count = 0;

		for (int i = 0; i < numOfVertices; i++) {
			int v = -1;
			for (int j = 0; j < numOfVertices; j++) {
				if (!visited[j] && (v == -1 || minEdges[j] < minEdges[v])) {
					v = j;
				}
			}
			if (v == -1 || minEdges[v] > INT_MAX) {
				printf("no spanning tree");
				free(visited);
				free(minEdges);
				free(selectedEdges);
				exit(0);
			}
			visited[v] = 1;

			if (selectedEdges[v] != -1) {
				ansEdge[count].start = v;
				ansEdge[count].finish = selectedEdges[v];
				count++;
			}
			for (int j = 0; j < numOfEdges; j++){
				if (edge[j].start == v || edge[j].finish == v){
					for (int to = 0; to < numOfVertices; to++)
						if (((edge[j].finish == to) || (edge[j].start == to)) && (edge[j].weight < minEdges[to])){
							minEdges[to] = edge[j].weight;
							selectedEdges[to] = v;
						}
				}
			}

		}
		for (int i = 0; i < count; i++)
			printf("%d %d\n", ansEdge[i].start + 1, ansEdge[i].finish + 1);
		free(visited);
		free(minEdges);
		free(selectedEdges);
	}
}

int main(){
	FILE* in = fopen("in.txt", "r");
	if (!in) return EXIT_FAILURE;
	int numOfVertices = 0;
	int numOfEdges = 0;

	if (fscanf(in, "%d", &numOfVertices) == EOF) {
		printf("bad number of lines");
		exit(0);
	}
	if (fscanf(in, "%d", &numOfEdges) == EOF) {
		printf("bad number of lines");
		exit(0);
	}
	Edge *edge = (Edge*)malloc(sizeof(Edge)*numOfEdges);
	checkAndRead(in, numOfVertices, numOfEdges, edge);
	fclose(in);
	AnsEdge *ansEdge = (AnsEdge*)malloc((numOfVertices - 1) * sizeof(AnsEdge));
	prim (edge, numOfVertices, numOfEdges, ansEdge);
	free(ansEdge);
	free(edge);
	return 0;
}