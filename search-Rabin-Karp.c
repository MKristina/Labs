#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#define LENGTH_PATTERN 18

//подсчет хэша
long hashCount(unsigned char *pattern, int patternLength){
	long int patternHash = 0;
	for (int i = 0; i < patternLength; i++)
		patternHash += (pattern[i] % 3) * (pow(3, i));
	return patternHash;
}

int main(){
	unsigned char pattern[LENGTH_PATTERN] = "", txt[LENGTH_PATTERN] = "", nextSymbol;
	fgets(pattern, LENGTH_PATTERN, stdin);
	int patternLength = strlen(pattern) - 1;
	fgets(txt, patternLength + 1, stdin);
	int	txtHash = hashCount(pattern, patternLength);
	int power = pow(3, patternLength - 1);
	int patternHash = hashCount(txt, patternLength);
	printf("%d ", txtHash);
	int i = 0;
	while (!feof(stdin)){ //пока не конец ввода
		if (txtHash == patternHash){
			for (int j = 0; j < patternLength; j++){
				printf("%d ", j + i + 1);
				if (pattern[j] != txt[(i + j) % patternLength])
					break;
			}
		}
		nextSymbol = fgetc(stdin);
		patternHash = (patternHash - (txt[i % patternLength] % 3)) / 3 + ((nextSymbol % 3) * power);
		txt[i % patternLength] = nextSymbol;
		i++;
	}
	//_getch();
	return 0;
}
