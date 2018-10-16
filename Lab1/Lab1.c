#include <stdio.h>
#include <string.h>
#include <conio.h>

#define ALPHABET 256
#define max(a,b) ((a) > (b) ? (a) : (b))

// заполнение таблицы смещений
void createShiftTable(char *pattern, int patternLength, int shiftTable[ALPHABET]) {
	for (int i = 0; i < ALPHABET; i++)
	{
		shiftTable[i] = patternLength;
	}
		
	for (int i = 0; i < patternLength - 1; i++)
	{
		shiftTable[pattern[i]] = patternLength - 1 - i;
	}
}

// алгоритм 
int B_M_Algorithm(char *txt, char *pattern, int temp) {
	
	int patternLength = strlen(pattern);
	int txtLength = strlen(txt);

	int shiftTable[ALPHABET];
	createShiftTable(pattern, patternLength, shiftTable);

	int i = patternLength - 1;
	int j = i;
	int k = i;
	while ( i <= txtLength - 1) {
		while (j >= 0){
			j = patternLength - 1;
			k = i;
	        printf("%d ", k + 1 + temp);
			while (j >= 0 && txt[k] == pattern[j]) {
				k--;
				j--;
				if (j >= 0)printf("%d ", k + 1 + temp);
			}
			if ((j >= 0) && (i <= txtLength - 1))	i += shiftTable[txt[i]]; else break;
		}

		if (j < 0) {
			j = patternLength - 1;
			i += patternLength ;
		} 
			
	}
	
	return 0;
}

int main() {

	char pattern[ALPHABET] = "";

	char txt[ALPHABET] = "";
	char text[1024]="";
	int temp = 0;
	gets(pattern);
	int patternLength = strlen(pattern);
	//gets(txt);
	int textLength = 0;
	while (fgets(txt, sizeof(txt), stdin)){	
		textLength = strlen(text);
		if (textLength + strlen(txt) < 1024){
			strcat(text, txt);
			if ((txt[0] == '\n') || (txt[0] == '\r' && txt[1] == '\n')){		
				break;
			}
		}
		/*else{ 
			
			if (patternLength >= 1 && patternLength <= 16 && patternLength <= textLength)
				B_M_Algorithm(text, pattern, temp);
			temp += 1024;
		strcpy(text, "");
		strcat(text, txt);

		}*/
	}	
	
	if (patternLength >= 1 && patternLength <= 16 && patternLength <= strlen(text))
		B_M_Algorithm(text, pattern, temp);
	//_getch();
	return 0;
}
