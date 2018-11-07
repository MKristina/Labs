#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>

int check(int *sequence, int N, int length) {
	int array[10] = { 0 };
	if ((N < 0) || (length > 10) || (length < 1)) {
		printf("bad input");
		return 0;
	}
	for (int i = 0; i < length; i++) {
		if (sequence[i] < 0 || sequence[i] > 9){
			printf("bad input");
			return 0;
		}
		if (array[sequence[i]] == 1){
			printf("bad input");
			return 0;
		}
		array[sequence[i]] = 1;
	}
	return 1;
}

void quck_sort(int *sequence, int first, int last){

	if (first < last){
		int left = first, right = last, supElement = sequence[(left + right) / 2];
		do{
			while (sequence[left] < supElement) left++;
			while (sequence[right] > supElement) right--;
			if (left <= right){
				int temp = sequence[left];
				sequence[left] = sequence[right];
				sequence[right] = temp;
				left++;
				right--;
			}
		} while (left <= right);
		quck_sort(sequence, first, right);
		quck_sort(sequence, left, last);
	}
}

void dijkstra(int* sequence, int N, int length){
	int flag = 0, count=0, i;
	while (count < N){
		flag = 0;
		
		for ( i = length - 2; i >= 0; i--){
			if (sequence[i] < sequence[i + 1]){
				flag = 1;
				break;
			}
		}
		if (flag == 0) {
			break;
		}
		int min = i + 1;
		for (int j = i + 1; j < length; j++){
			if (sequence[j]<sequence[i + 1] && sequence[j]>sequence[i])
				min = j;
		}
		char temp = sequence[i];
		sequence[i] = sequence[min];
		sequence[min] = temp;
		quck_sort(sequence, i + 1, length-1);
		count++;
		for (i = 0; i < length; i++)
			printf("%d", sequence[i]);
		printf("\n");
	}
}
int main(){
	char str[20] = "";
	int N = 0, *sequence; 
	scanf("%s", str);
	scanf("%d", &N);
	int strLength = strlen(str);
	sequence = (int*)malloc(strLength * sizeof(int));
	for (int i = 0; i < strLength; i++){
		sequence[i] = str[i] - '0';
	}
	if (check(sequence, N, strLength)){
		dijkstra(sequence, N, strLength);
	}
	free(sequence);
	//_getch();
	return 0;
}
