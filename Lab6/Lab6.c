#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

void quck_sort(int *sequence, int first, int last){

	if (first < last){
		int left = first, right = last, supElement = sequence[(left + right) / 2]; //границы подмассивов,опорный элемент
		do{
			while (sequence[left] < supElement) left++; //левая граница сдвигается вправо
			while (sequence[right] > supElement) right--; //правая сдвигается влево
			
			if (left <= right){
				int temp = sequence[left]; //обмен местами элементов, сдвиг границ
				sequence[left] = sequence[right];
				sequence[right] = temp;
				left++;
				right--;
			}
		} while (left <= right);
		//рекурсивный вызов
		quck_sort(sequence, first, right);
		quck_sort(sequence, left, last);
	}

}


int main(void)
{
	int *sequence;
	int N, first, last;
	scanf("%d", &N);
	sequence = (int*)malloc(N * sizeof(int));
	for (int i = 0; i < N; i++){
		scanf("%d", &sequence[i]);
	}
	first = 0;
	last = N - 1;
	quck_sort(sequence, first, last);
	for (int i = 0; i<N; i++){
		printf("%d ", sequence[i]);
	}
	free(sequence);
	return 0;
}
