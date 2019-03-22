#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#define MAX_VERTICES 5000  
#define INF UINT_MAX

void checkAndRead(FILE *in, int numOfVertices, int numOfEdges, int start, int finish, unsigned **edges) {
	if (start < 0 || start > numOfVertices - 1 || finish < 0 || finish > numOfVertices - 1){
		printf("bad vertex\n");
		exit(0);
	}
	if (numOfEdges < 0 || numOfEdges >(numOfVertices*(numOfVertices - 1) / 2)) {
		printf("bad number of edges");
		exit(0);
	}
	unsigned c = 0;
	int a, b;
	for (int i = 0; i < numOfEdges; i++) {
		if (fscanf(in, "%d %d %u", &a, &b, &c) != EOF) {
			a--;
			b--;
		}
		else {
			printf("bad number of lines");
			exit(0);
		}
		if (c < 0 || c > INT_MAX) {
			printf("bad length");
			exit(0);
		}
		if (a < 0 || a > numOfVertices - 1 ||b < 0 || b > numOfVertices - 1) {
			printf("bad vertex");
			exit(0);
		}
		edges[a][b] = c;	
		edges[b][a] = c;
	}
}
void dijkstra(int numOfVertices, int start, int finish, unsigned **edges){

	int *visited = malloc(numOfVertices*sizeof(int*));
	unsigned *distance = malloc(numOfVertices*sizeof(unsigned*));
	int *parent = malloc(numOfVertices*sizeof(int*));
	unsigned *overflow = malloc(numOfVertices*sizeof(unsigned*));
	for (int i = 0; i < numOfVertices; i++){
		visited[i] = 0;
		distance[i] = INF;
		parent[i] = -1;
		overflow[i] = 0;
	}
	distance[start] = 0;
	for (int i = 0; i < numOfVertices; i++){
		unsigned minIndex = -1;
		for (int j = 0; j < numOfVertices; j++){
			if (!visited[j] && (minIndex == -1 || distance[j] < distance[minIndex]))
				minIndex = j;
		}
		if (distance[minIndex] == INF) break;
		visited[minIndex] = 1;

		for (int j = 0; j < numOfVertices; j++){
			if (edges[minIndex][j] == INF)
				continue;
			int to = j;
			int len = edges[minIndex][to];
			if ((overflow[minIndex] < overflow[to]) || (distance[minIndex] + len < distance[to]))
			{
				distance[to] = distance[minIndex] + len;
				overflow[to] = overflow[minIndex];
				while (distance[to] > INT_MAX){
					++overflow[to];
					distance[to] -= INT_MAX;
				}
				parent[to] = minIndex;
			}
		}
	}

	for (int i = 0; i < numOfVertices; i++)
	{
		if (distance[i] == INF)
			printf("oo ");
		else if (overflow[i])
			printf("INT_MAX+ ");
		else
			printf("%u ", distance[i]);
	}
	printf("\n");
	unsigned int overflowCount = 0;
	if (overflow[finish]){
		for (int i = 0; i < numOfVertices; i++){
			if (edges[finish][i] != INF && distance[i] + edges[finish][i] > INT_MAX)
				overflowCount++;
			if (overflowCount >= 2)
				break;
		}
	}
	if (start == finish){
		printf("%d\n", start + 1);
	}
	else if (distance[finish] == INF){
		printf("no path");
	}
	else if (overflowCount >= 2)
	{
		printf("overflow\n");
	}
	else {
		for (int i = finish; i != start; i = parent[i])
			printf("%d ", i + 1);
		printf("%d\n", start + 1);
	}
	free(visited);
	free(parent);
	free(overflow);
	free(distance);
}

int main(){
	FILE* in = fopen("in.txt", "r");
	if (!in) return EXIT_FAILURE;

	int numOfVertices = 0;
	int numOfEdges = 0;
	int start = 0;
	int finish = 0;

	if (fscanf(in, "%d", &numOfVertices) == EOF) {
		printf("bad number of lines");
		exit(0);
	}
	if (fscanf(in, "%d %d", &start, &finish) == EOF) {
		printf("bad number of lines");
		exit(0);
	}
	start--;
	finish--;
	if (fscanf(in, "%d", &numOfEdges) == EOF) {
		printf("bad number of lines");
		exit(0);
	}
	if (numOfVertices < 0 || numOfVertices > MAX_VERTICES) {
		printf("bad number of vertices");
		exit(0);
	}
	unsigned int **edges = (int**)malloc(numOfVertices*sizeof(unsigned int*));
	for (int i = 0; i < numOfVertices; i++){
		edges[i] = (int*)malloc(numOfVertices*sizeof(unsigned int));
		for (int j = 0; j < numOfVertices; j++){
			edges[i][j] = INF;
		}
	}
	checkAndRead(in, numOfVertices, numOfEdges, start, finish, edges);
	dijkstra(numOfVertices, start, finish, edges);
	for (int i = 0; i < numOfVertices; i++){
		free(edges[i]);
	}
	free(edges);
	fclose(in);
	return 0;
}