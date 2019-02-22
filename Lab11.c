#include <stdio.h>
#include <stdlib.h>

// ћаксимальное количество вершин
#define MAX_VERTICES 5000  

// –ебра графа
typedef struct edge {
	int start;
	int finish;
	int weight;      
} Edge;

void checkAndRead(FILE *in, int numOfVertices, int numOfEdges, Edge *edge) {
	if (numOfVertices < 0 || numOfVertices > MAX_VERTICES) {
		printf("bad number of vertices");
		exit(0);
	}
	if (numOfEdges < 0 || numOfEdges >(numOfVertices*(numOfVertices - 1) / 2)) {
		printf("bad number of edges");
		exit(0);
	}
	for (int i = 0; i < numOfEdges; i++) {
		if (fscanf(in, "%d %d %d", &edge[i].start, &edge[i].finish, &edge[i].weight) != EOF) {
			edge[i].start = edge[i].start - 1;
			edge[i].finish = edge[i].finish - 1;
		}
		else {
			printf("bad number of lines");
			exit(0);
		}
		if (edge[i].start < 0 || edge[i].start > numOfVertices-1 || edge[i].finish < 0 || edge[i].finish > numOfVertices-1) {
			printf("bad vertex");
			exit(0);
		}
		if (edge[i].weight < 0 || edge[i].weight > INT_MAX) {
			printf("bad length");
			exit(0);
		}
	}
}
void swap(int * a, int * b){
	*a ^= *b;
	*b ^= *a;
	*a ^= *b;
}
// сравнение двух ребер
int edgeCmp(const void *a, const void *b)
{
	return ((*(Edge*)a).weight - (*(Edge*)b).weight);
}
//добавление вершины в дерево, состо€щее только из этой вершины
void creationTree(int vertice, int* parent, int* rank) {
	parent[vertice] = vertice;
	rank[vertice] = 0;
}
//определ€етс€, в каком дереве находитс€ данна€ вершина
int findTree(int vertice, int* parent) {
	if (vertice == parent[vertice])
		return vertice;
	return parent[vertice] = findTree(parent[vertice], parent);
}
//объединение двух деревьев
void unionTree(int a, int b, int* parent, int* rank) {
	a = findTree(a, parent);
	b = findTree(b, parent);
	if (a != b) {
		if (rank[a] < rank[b])
			swap(&a, &b);
		parent[b] = a;
		if (rank[a] == rank[b])
			++rank[a];
	}
}

int main(){
	FILE* in = fopen("in.txt", "r");
	FILE* out = fopen("out.txt", "w");
	if (!in || !out) return EXIT_FAILURE;

	int numOfVertices = 0;
	int numOfEdges = 0;

	if (fscanf(in, "%d", &numOfVertices) == EOF) {
		printf("bad number of lines");
		_getch();
		exit(0);
	}
	if (fscanf(in, "%d", &numOfEdges) == EOF) {
		printf("bad number of lines");
		_getch();
		exit(0);
	}
	
	Edge *edge = (Edge*)malloc(sizeof(Edge)*numOfEdges);
	int *parent = (int*)malloc(sizeof(int)*numOfVertices);
	int *rank = (int*)malloc(sizeof(int)*numOfVertices);

	checkAndRead(in, numOfVertices, numOfEdges, edge);

	int visitedVertices = 1;

	// —ортировка ребер в пор€дке возрастани€ весов
	qsort(edge, numOfEdges, sizeof(Edge), edgeCmp);

	//–азмещение каждой вершины в своЄ дерево
	for (int i = 0; i < numOfVertices; i++)
		creationTree(i, parent, rank);

	//ѕроход по всем ребрам. ќпредел€етс€, наход€тс€ ли начало и конец в разных деревь€х
	for (int i = 0; i < numOfEdges; i++) {
		Edge currentEdge = edge[i];
		if (findTree(currentEdge.start, parent) != findTree(currentEdge.finish, parent)) {
			printf("%d %d\n", currentEdge.start+1, currentEdge.finish+1);
			unionTree(currentEdge.start, currentEdge.finish, parent, rank);	
			visitedVertices++;
		}
	}
	// если в остове графа не все вершины, то у него нет ни одного каркаса 
	if (visitedVertices != numOfVertices) {
		rewind(out);
		fprintf(out, "no spanning tree\n");
	}
	free(edge);
	free(parent);
	free(rank);
	return 0;
}