#include<conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

//создание и восстановление пирамиды
void createHeap(int *sequence, int index, int Num, int elem){
	int child;
	while (index < (Num / 2)){
		child = (index + 1) * 2 - 1;
		if ((child < Num - 1) && (sequence[child] < sequence[child + 1])) child++;
		if (elem >= sequence[child]) break;
		sequence[index] = sequence[child];
		index = child;
	}
	sequence[index] = elem;
}

//алгоритм сортировки
void heapSort(int sequence[], int Num){
	int elem;

	for (int i = (Num / 2) - 1; i >= 0; i--)
		createHeap(sequence, i, Num, sequence[i]);
	for (int i = Num - 1; i >= 0; i--){
		elem = sequence[i]; 
		sequence[i] = sequence[0];
		createHeap(sequence, 0, i, elem);
	}
}
int main(){
	int *sequence;
	int N, number;
	scanf("%d", &N);
	sequence = (int*)malloc(N * sizeof(int));
	for (int i = 0; i < N; i++)
		scanf("%d", &sequence[i]);	
	heapSort(sequence, N);
	for (int i = 0; i < N; i++)
		printf(" %d", sequence[i]);
	free(sequence);
	//_getch();
	return 0;
}
	


	
