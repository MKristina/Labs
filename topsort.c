#define _CRT_SECURE_NO_WARNINGS
#define MAX_VERTICES 1000
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

void checkInput(int numOfVertices, int numOfEdges)
{
	if (numOfVertices > MAX_VERTICES || numOfVertices < 0) {
		printf("bad number of vertices");
		exit(0);
	}
	if (numOfEdges > numOfVertices*(numOfVertices + 1) / 2 || numOfEdges < 0) {
		printf("bad number of edges");
		exit(0);
	}
}
int checkVertex(int start, int finish, int numOfVertices)
{
	if (start > numOfVertices || start  < 0 || finish > numOfVertices || finish < 0){
		printf("bad vertex");
		return -1;
	}
	else if (start == finish){
		printf("impossible to sort");
		return -1;
	}
	return 1;
}
short** initGraph(short** graph, int numOfVertices)
{
	for (int i = 0; i < numOfVertices; i++)
		for (int j = 0; j < numOfVertices; j++) 
			graph[i][j] = 0;
	return graph;
}
void freeGraph(short **graph, int numOfVertices)
{
	for (int i = 0; i < numOfVertices; i++)	
		free(graph[i]);
	free(graph);
}
void topSort(short** graph, int numOfVertices, int* column, int* sortedVertices){
	int j = 0;
	int cntSortVertices = 0, cntNotSortVertices = 0;
	// пока не все вершины отсротированы и кол-во вершин, в которые входят ребра, не равно общему кол-ву вершин
	while ((cntSortVertices != numOfVertices) && (cntNotSortVertices != numOfVertices)) {
		if (column[j] == 0){ // нет ребер, входящих в данную вершину
			sortedVertices[cntSortVertices] = j + 1; 
			cntSortVertices++;
			for (int i = 0; i < numOfVertices; i++){
				if (graph[j][i] == 1) {
					graph[j][i] = 0;
					column[i]--;
				}
			}
			cntNotSortVertices = 0;
			column[j] = 2;
		}
		else cntNotSortVertices++;
		j = (j + 1) % numOfVertices;
	}
	if (cntSortVertices != numOfVertices){
		printf("impossible to sort");
		exit(0);
	}
	else for (int i = 0; i < cntSortVertices; i++)
		printf("%d ", sortedVertices[i]);
}

int main() {

	int numOfVertices, numOfEdges;
	int start, finish;
	FILE* in = fopen("in.txt", "r");
	fscanf(in, "%d", &numOfVertices);
	if (fscanf(in, "%d", &numOfEdges) == EOF) {
		printf("bad number of lines");
		exit(0);
	}
	checkInput(numOfVertices, numOfEdges);
	//если сортировка не нужна
	if ((numOfEdges == 1 && fscanf(in, "%d%d", &start, &finish) != EOF) || (numOfEdges == 0) && fscanf(in, "%d%d", &start, &finish) == EOF){ 
		if ((numOfEdges == 1) && (checkVertex(start, finish, numOfVertices) == -1)) return 0;
		for (int i = 0; i < numOfVertices; i++) printf("%d ", i + 1);
		exit(0);
	}
	//количество ребер, входящих в вершину
	int* column = malloc(numOfVertices*sizeof(int));
	//граф
	short** graph = malloc(numOfVertices*sizeof(short*));
	for (int i = 0; i < numOfVertices; i++){
		graph[i] = (short*)malloc(numOfVertices*sizeof(short));
		column[i] = 0;
	}
	graph = initGraph(graph, numOfVertices);
	int lines = 2;
	while (fscanf(in, "%d%d", &start, &finish) != EOF){
		if (checkVertex(start, finish, numOfVertices) == -1) {
			freeGraph(graph, numOfVertices);
			free(column);
			exit(0);
		}
		graph[start - 1][finish - 1] = 1;
		column[finish - 1]++;
		lines++;
	}
	fclose(in);
	if (lines < numOfEdges + 2){
		freeGraph(graph, numOfVertices);
		free(column);
		printf("bad number of lines");
		exit(0);
	}
	int* sortedVertices = malloc(numOfVertices*sizeof(int));
	topSort(graph, numOfVertices, column, sortedVertices);
	freeGraph(graph, numOfVertices);
	free(column);
	free(sortedVertices);
	return 0;
}