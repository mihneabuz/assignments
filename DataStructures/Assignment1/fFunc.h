//Buzatu-Pahontu Mihnea-Nicolae
//315CB
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structures.h"

//Declarations
void FreeTable(t_table *T);
void FreeLineInt(t_intLine *Line);
void FreeLineFloat(t_floatLine *Line);
void FreeLineString(t_stringLine *Line);
void PrintLine(void *Line, t_cellType Type);
void PrintColumns(t_table *T);

t_table *InitTable(char *TABLE_NAME, t_cellType DATA_TYPE)
{
	//Initializeaza un tabel si returneaza pointer spre el
	t_table *T;
	T = malloc(sizeof(t_table));
	if (T == NULL) return NULL;
	memcpy(T->name, TABLE_NAME, MAX_TABLE_NAME_LEN);
	T->type = DATA_TYPE;
	T->columns = NULL;
	T->lines = NULL;
	T->next = NULL;
	return T;
}

int AddTable(t_db *DB, t_table *T)
{
	//Adauga un tabel T la baza de date DB
	if (DB->tables)
	{
		t_table *p;
		for (p = DB->tables; p->next != NULL; p = p->next)
			if (!strcmp(p->name, T->name)) return 0;
		if (!strcmp(p->name, T->name)) return 0;
		p->next = T;
	}
	else DB->tables = T;
	return 1;
}

int DeleteTable(t_db *DB, char *TABLE_NAME)
{
	//Sterge din baza de date DB tabelul cu numele TABLE_NAME
	if (DB->tables)
	{
		t_table *p, *tmp = NULL;
		for (p = DB->tables; p != NULL; tmp = p, p = p->next)
		{
			if (!strcmp(p->name, TABLE_NAME))
			{
				if (tmp != NULL) tmp->next = p->next;
				else DB->tables = p->next;
				FreeTable(p);
				return 1;
			}
		}
	}
	return 0;
}

int getType(char *DATA_TYPE)
{
	//returneza 0 pt INT, 1 pt FLOAT, 2 pt STRING, -1 pt tip gresit
	if (!strcmp(DATA_TYPE, "INT")) return 0;
	if (!strcmp(DATA_TYPE, "FLOAT")) return 1;
	if (!strcmp(DATA_TYPE, "STRING")) return 2;
	return -1;
}

t_table *getTable(t_db *DB, char *TABLE_NAME)
{
	//returneza un pointer spre tabelul cu numele TABLE_NAME
	if (DB->tables)
	{
		t_table *p;
		for (p = DB->tables; p != NULL; p = p->next)
			if (!strcmp(p->name, TABLE_NAME)) return p;
		return NULL;
	}
	else return NULL;
}

int AddColumn(t_table *T, char *COLUMN)
{
	//Adauga o coloana la tabelul T, folosit pentru comanda CREATE
	t_column *aux = malloc(sizeof(t_column));
	if (aux == NULL) return 0;
	memcpy(aux->name, COLUMN, MAX_COLUMN_NAME_LEN);
	aux->next = NULL;
	if (T->columns == NULL) T->columns = aux;
	else
	{
		t_column *p;
		for (p = T->columns; p->next != NULL; p = p->next);
		p->next = aux;
	}
	return 1;
}

char *NewLine(t_cellType Type)
{
	//aloca o linie noua si returneaza pointer spre ea
	if (Type == 0)
	{
		t_intLine *Line = malloc(sizeof(t_intLine));
		Line->next = NULL;
		Line->cells = NULL;
		return (char*)Line;
	}
 	if (Type == 1)
	{
		t_floatLine *Line = malloc(sizeof(t_floatLine));
		Line->next = NULL;
		Line->cells = NULL;
		return (char*)Line;
	}
	if (Type == 2)
	{
		t_stringLine *Line = malloc(sizeof(t_stringLine));
		Line->next = NULL;
		Line->cells = NULL;
		return (char*)Line;
	}
	return NULL;
}

char *NewElem(t_cellType Type)
{
	//aloca o celula noua si returneaza pointer spre ea
	if (Type == 0)
	{
		int x;
		scanf("%d", &x);
		t_intCell *Cell = malloc(sizeof(t_intCell));
		Cell->value = x;
		Cell->next = NULL;
		return (char*)Cell;
	}
	if (Type == 1)
	{
		float y;
		scanf("%f", &y);
		t_floatCell *Cell = malloc(sizeof(t_floatCell));
		Cell->value = y;
		Cell->next = NULL;
		return (char*)Cell;
	}
	if (Type == 2)
	{
		t_stringCell *Cell = malloc(sizeof(t_stringCell));
		Cell->value = malloc(sizeof(char)*MAX_COLUMN_NAME_LEN);
		scanf("%s", Cell->value);
		Cell->next = NULL;
		return (char*)Cell;
	}
	return NULL;
}

void AddLine(t_table *T)
{
	//adauga o linie noua la tabelul T
	void *Line;
	if (T->lines)
	{
		//gaseste ultima linie a tabelului T
		void *p, *tmp;
		for (p = T->lines; p != NULL; p = *(char**)(p+sizeof(void*)))
				tmp = p;
		Line = *(char**)(tmp+sizeof(void*)) = NewLine(T->type);	
	}
	else Line = T->lines = NewLine(T->type);
	t_column *q;
	int first = 1;
	for (q = T->columns; q != NULL; q = q->next)
	{
		//Pentru fiecare coloana, citeste un elem si il adauga la linie
		if (first)
		{
			*(char**)Line = NewElem(T->type);
			Line = *(char**)Line;
			first = 0;
		}
		else 
		{
			*(char**)(Line+sizeof(void*)) = NewElem(T->type);
			Line = *(char**)(Line+sizeof(void*));
		}
	}
}

int CompareExp(float value, char *EXP)
{	
	//aplica expresia citita functie CompareValue
	if (!strcmp(EXP, "<")) return (value < 0);
	if (!strcmp(EXP, "<=")) return (value <= 0);
	if (!strcmp(EXP, "==")) return (value == 0);
	if (!strcmp(EXP, "!=")) return (value != 0);
	if (!strcmp(EXP, ">=")) return (value >= 0);
	if (!strcmp(EXP, ">")) return (value > 0);
	return 0;
}

float CompareValue(void *a, void *b, t_cellType Type)
{
	//compara doua valori in functie de tipul de date
	if (Type == 0) return (*(int*)a-*(int*)b);
	if (Type == 1) return (*(float*)a-*(float*)b);
	if (Type == 2) return ((float)strcmp(*(char**)a, (char*)b));
	return 0;
}

int EditLines(t_table *T, char *COLUMN, char *EXP, char *mod)
{
	//combina comenzile de SEARCH si DELETE intr-o singura functie
	//mod - poate fi SEARCH sau DELETE
	//cauta coloana cu numele COLUMN in tabelul T si daca o gaseste, 
	//retine indicele, dupa care parcurge toate liniile pana la indicele
	//respectiv si verifica conditia (el EXP value) dupa care aplica
	//PrintLine respectiv Delete linie
	int MODE;
	if (!strcmp(mod, "SEARCH")) MODE = 0;
	else if (!strcmp(mod, "DELETE")) MODE = 1;

	//citeste elementul in functie de tip
	void *el;	
	if (T->type == 0)
	{
		el = malloc(sizeof(int));
		scanf("%d", (int*)el);
	}
	if (T->type == 1)
	{
		el = malloc(sizeof(float));
		scanf("%f", (float*)el);
	}
	if (T->type == 2)
	{
		el = malloc(MAX_COLUMN_NAME_LEN * sizeof(char));
		scanf("%s", (char*)el);
	}

	//gaseste coloana cu numele COLUMN
	t_column *p;
	int nr = 1;
	for (p = T->columns; p != NULL; p = p->next, nr++)
		if (!strcmp(p->name, COLUMN)) break;
	if (!p) 
	{
		free(el);
		return 0;
	}

	if (MODE == 0) PrintColumns(T);
	void *q, *r, *prev = NULL, *tmp = NULL;
	int i;
	q = T->lines;
	while (q != NULL)
	{	
		//parcurge liniile, dupa parcurge celulele pana ajunge la coloana dorita
		for (r = *(char**)q, i = 1; i != nr; r = *(char**)(r+sizeof(void*)), i++);
		if (MODE == 0)
		{
			//Pentru afisare (SEARCH)
			if (CompareExp(CompareValue(r, el, T->type), EXP))
				PrintLine(q, T->type);
			q = *(char**)(q+sizeof(void*));
		}
		else if (MODE == 1)
		{
			//Pentru stergere (DELETE)
			if (CompareExp(CompareValue(r, el, T->type), EXP)) 
			{
				tmp = q;
				q = *(char**)(q+sizeof(void*));
				if (T->type == 0) FreeLineInt((t_intLine*) tmp);
				if (T->type == 1) FreeLineFloat((t_floatLine*) tmp);
				if (T->type == 2) FreeLineString((t_stringLine*) tmp);
				if (prev) *(char**)(prev+sizeof(void*)) = q;
				else T->lines = q;
			}
			else
			{
				prev = q;
				q = *(char**)(q+sizeof(void*));
			}
		}
		
	}	
	free(el);
	return 1;
}
