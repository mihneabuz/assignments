#include "TStiva.h"

void afisInt(void* a) {
	printf("%d", *(int*)a);
}

void afisString(void* a) {
	printf("%c", *(char*)a);
}

int cmpInt(void *a, void *b) {
	return *(int*)a - *(int*)b;
}

/* returneaza prioritatea simbolului */
int getPriority(char symbol) {

	switch (symbol) {
		case '/':
		case '*':
			return 2;

		case '+':
		case '-':
			return 1;

		default:
			return 0;
	}
}

/*
	returneaza stringul "<operand1> <op> <operand2>"
	ajuta la generarea unei forme infixate
 */
char *SmartConcatInfix(char *operand1, char *op, char *operand2) {
	char *result = (char *)calloc(strlen(operand1) +
					  	strlen(operand2) + strlen(op) + 4,
				      	sizeof(char));
	
	strcat(result, operand1);
	strcat(result, " ");
	strcat(result, op);
	strcat(result, " ");
	strcat(result, operand2);

	free(operand1);
	free(operand2);
	free(op);

	return result;
}

/*
	returneaza stringul "<equation> <op>"
	ajuta la generarea unei forme postfixate
 */
void SmartConcatPostfix(char *equation, char *op) {
	strcat(equation, op);
	strcat(equation, " ");
	free(op);
}

/* TODO EX 1 */
char *PrefixToInfix(char *prefix)
{
	int i;
	char *tmp, *op1, *op2, *op;
	TStiva s = InitS();
	for (i = strlen(prefix)-1; i >= 0; i-=2)
	{
		if (getPriority(prefix[i]) == 0)
		{
			tmp = malloc(4*sizeof(char));
			*tmp = *(prefix+i);
			*(tmp+1) = '\0';
			Push(s, tmp);
		}
		else
		{
			op = malloc(4*sizeof(char));
			*op = *(prefix+i);
			*(op+1) = '\0';
			op1 = Pop(s);
			op2 = Pop(s);
			op = SmartConcatInfix(op1, op, op2);
			Push(s, op);
		}		
	}
	char *res = Pop(s);
	free(s);
	return res;	
}


int main() {
//	char postfix[30] = "a b * c d / + e f * g * - h -";
//	char infix[30] = "a * b + c / d - e * f * g - h";
	char prefix[30] = "+ * a b - / c d - * * e f g h";

	/* Exemplu de apeluri functii pe stiva */
/*	TStiva s = InitS();
	int vec[5] = {2,3,1,5,4};
	int *x;

	for (int i = 0; i < 5; i++) {
		elementele puse pe stiva trebuie alocate pe heap
		x = malloc(sizeof(int));
		*x = vec[i];
		Push(s, x);
	}

	AfisareS(s, afisInt);

	afisInt(Top(s));		// print 4
	printf(" TOP\n");
	afisInt(Pop(s));		// print 4
	printf(" POP\n");
	afisInt(Top(s));		// print 5 (noul varf)
	printf(" NEW TOP\n");

	SortareS(s, cmpInt);	// merge doar daca a fost implementata
	AfisareS(s, afisInt);
	DistrS(s);*/
	
	char *res = PrefixToInfix(prefix);
	printf("Expresia infixata este: %s\n", res);
	free(res);
	/* apeluri functii conversie */

    return 1;
}
