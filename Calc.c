#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct list {
	int data;
	struct list *next;
};
typedef struct stack { struct list *top; } Stack;
struct stack *create() {
	Stack *S;
	S = (Stack*)malloc(sizeof(Stack));
	S->top = NULL;
	return S;
}
int top(Stack *S) {
	if (S->top) return (S->top->data);
	else return -1;
}
int empty(Stack *S) {
	return (S->top == NULL);
}
int Pop(Stack *S) {
	int a;
	struct list *p;
	p = S->top;
	a = p->data;
	S->top = p->next;
	free(p);
	return a;
}
void Push(int a, Stack *S) {
	struct list *p;
	p = (struct list*) malloc(sizeof(struct list));
	if (p) {
		p->data = a;
		p->next = S->top;
		S->top = p;
	}
}
int Prior(char c) {
	switch (c) {
	case '*': return 4;
	case '/': return 4;
	case '+': return 3;
	case '-': return 3;
	case ')': return 2;
	case '(': return 1;
	default: return 0;
	}
}
int Operators(char c) {
	switch (c) {
	case '*': case '/': case '+': case '-': return 2;
	case ')': case '(': case '=': case ' ': return 1;
	default: return 0;
	}
}
int Check(char *InputStr) {
	int numBrackers = 0;
	int number = 0;
	int bracket = 0;
	if (strlen(InputStr) < 3)
		return 0;
	for (int i = 0; i < strlen(InputStr); i++) {
		if (InputStr[i] == ' ') i++;
		if ((Operators(InputStr[i]) == 0) && ((InputStr[i] < '0') || (InputStr[i] > '9')))
			return 0;
		if (InputStr[i] == '(') {
			if (number) return 0;
			else {
				numBrackers++;
				bracket = 1;
			}
		}
		if (InputStr[i] == ')') {
			bracket = 0;
			if (!number) return 0;
			else
				numBrackers--;
			if (numBrackers < 0) return 0;
		}
		if ((InputStr[i] >= '0') && (InputStr[i] <= '9')) {
			if (number)	return 0;
			if ((i + 1 < strlen(InputStr)) && ((InputStr[i+1] < '0') || (InputStr[i+1] > '9'))) {
				number = 1;
				bracket = 0;
			}
		}
		if (Operators(InputStr[i]) == 2) {
			if ((bracket) && (InputStr[i] != '-')) return 0;
			number = 0;
			bracket = 0;
		}
		if ((i >= strlen(InputStr) - 1) && (Operators(InputStr[i]) == 2)) 
			return 0;
		if ((i < strlen(InputStr)) && (Operators(InputStr[i]) == 2) && (Operators(InputStr[i+1]) == 2))
			return 0;
	}
	if (numBrackers > 0) return 0;
	return 1;
}

char *Conversation(char *InputStr){
	char StrOut[1501] = "";
	int i = 0, j = 0;
	Stack *S = create();
	for (i = 0; InputStr[i] != '\0'; i++){
		switch (Prior(InputStr[i])) {
		case 0:
			if ((InputStr[i] >= '0') && (InputStr[i] <= '9')){
				StrOut[j] = InputStr[i];
				j++;
			}
			if (Prior(InputStr[i + 1]) != 0 || (InputStr[i + 1] == '\0')){
				StrOut[j] = ' ';
				j++;
			}
			break;
		case 1:
			Push(InputStr[i], S);
			break;
		case 2:
			while (top(S) != '(') {
				StrOut[j] = Pop(S);
				j++;
			}
			Pop(S);
			break;
		case 3:
			while (Prior(top(S)) >= Prior(InputStr[i])) {
				StrOut[j] = Pop(S);
				j++;
			}
			Push(InputStr[i], S);
			break;
		case 4:
			while (Prior(top(S)) >= Prior(InputStr[i])) {
				StrOut[j] = Pop(S);
				j++;
			}
			Push(InputStr[i], S);
			break;
		}
	}
	while (!empty(S)) {
		if (top(S) != '(') {
			StrOut[j] = Pop(S);
			j++;
		}
	}
	free(S);
	return &StrOut[0];
}

void Count(char *Str){
	int divZero = 1;
	int result = 0;
	char *InputStr = Conversation(Str);
	Stack *S = create();
	for (int i = 0; i < strlen(InputStr); i++)
	{
		if ((InputStr[i] >= '0') && (InputStr[i] <= '9'))
		{
		    int num = 0;
			while (InputStr[i]!=' ' && !Operators(InputStr[i]))
			{
				num = (num * 10) + (InputStr[i] - '0');
				i++;
				if (i == strlen(InputStr)) break;
			}
			i--;
			Push(num, S);
		}
		if ((Operators(InputStr[i]))==2 )
		{
			int a = Pop(S);
			if (!empty(S)){
				int b = Pop(S);
				switch (InputStr[i])
				{
				case '-': result = b - a; break;
				case '+': result = b + a; break;
				case '*': result = b * a; break;
				case '/':if (a != 0) result = (b / a);
						 else divZero = -1;
					 break;
				default:  break;
				}
			Push(result, S);
			}
		}
	}
	if (divZero == -1) printf("division by zero");
	else printf("%d\n", Pop(S));
	free(S);
}
int main() {
	char InputStr[1501] = "";
	gets(InputStr);
	if (!Check(InputStr))
		printf("syntax error");
	else
	 Count(InputStr);
	return 0;
}
