#include<stdio.h>
#include <string.h>
#include <conio.h>

// Целую часть числа из исходной с.с. в десятичную.
long long  IntToDec(char *number, int b, int length, int &decPart) {
	long long  number2 = 0;
	for (int i = 0; i < length; i++) {
		if (number[i] == '.') {
			decPart = i + 1;
			break;
		}
		if ((number[i] >= 0x41) && (number[i] <= 0x46))
			number2 = number2 * b + (number[i] - 0x41 + 10);
		else
			number2 = number2 * b + (number[i] - 0x30);
	}
	return number2;
}
// Десятичную часть числа из исходной с.с. в десятичную.

double IntToDecFloat(char *number, int b1, int length, int decPart) {
	double  number2 = 0;
	for (int i = length - 1; i >= decPart; i--) {
		if ((number[i] >= 0x41) && (number[i] <= 0x46))
			number2 = (number2 + (number[i] - 55)) / b1;
		else
			number2 = (number2 + (number[i] - 0x30)) / b1;
	}
	return number2;
}

// Целую часть числа из десятичной с.с. в заданную.
int DecToB2(long long number, int b2, char *number2) {
	int i = 0;
	if (number == 0)	number2[0] = 0x30;
	else {
		while (number != 0) {
			if (((number%b2) >= 10) && ((number%b2) <= 15))
				number2[i] = (number%b2) + 55;
			else
				number2[i] = (number%b2) + 0x30;
			number = number / b2;
			i++;
		}
	}
	int length = strlen(number2);
	for (i = length - 1; i >= 0; i--)
		printf("%c", number2[i]);
	return 0;
}

// Десятичную часть числа из десятичной с.с. в заданную.
int DecToB2Float(double number, int b2, char *number2) {
	int i = 0, n = 0;
	printf(".");
	if (number != 0) {
		while ((i < 12)) {
			n = int(number*b2);
			if ((n >= 10) && (n <= 15))
				number2[i] = n + 0x41 - 10;
			else
				number2[i] = n + 0x30;
			if (number != 0) printf("%c", number2[i]);
			number = (number*b2) - n;
			i++;
		}
	}
	return 0;
}
bool check(char *number, int b1, int b2, int length) {
	char abc[] = { '.', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
	int i = 0, j = 0, ch = 0, dot = 0;
	if (b1 < 2 || b1>16 || b2 < 2 || b2>16) {
		printf("bad input");
		return false;
	}
	if (length > 13) {
		printf("bad input");
		return false;
	}
	for (i = 0; i < length; i++) {
		if (number[i] == '.') {
			if (dot == 1 || i == 0 || i == length - 1) {
				printf("bad input");
				return false;
			}
			if (dot == 0) dot = 1;
		}
		for (j = 0; j < b1 + 1; j++) {
			if (number[i] >= 'a' && number[i] <= 'f') number[i] -= 32;
			if (number[i] == abc[j])
				ch++;
		}
	}
	if (ch != i) {
		printf("bad input");
		return false;
	}

	return true;
}
int main(int argc, char *argv[])
{
	int b1, b2;
	int length = 0, decPart = 0;
	char number[256], number2[256] = "", flNumber[256] = "";
	scanf("%d%d", &b1, &b2);
	scanf("%s", number);
	length = strlen(number);
	if (check(number, b1, b2, length)) {
		double n;
		DecToB2(IntToDec(number, b1, length, decPart), b2, number2);
		if (decPart > 0)  	
			DecToB2Float(IntToDecFloat(number, b1, length, decPart), b2, flNumber);
	}
//_getch();
	return 0;
}

