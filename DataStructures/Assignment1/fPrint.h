//Buzatu-Pahontu Mihnea-Nicolae
//315CB
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "structures.h"

//Sursa cu functiile de afisare

//padding pentru afisare
const char PADDING[31] = "------------------------------";

void PrintLine(void *Line, t_cellType Type)
{
	//afiseaza o linie, primeste pointer la linie
	void *p;
	if (*(char**)Line)
	{
		for (p = *(char**)Line; p != NULL; p = *(char**)(p+sizeof(void*)))
		{
			if (Type == 0) printf("%-30d ", *(int*)p);
			else if (Type == 1) printf("%-30f ", *(float*)p);
			else if (Type == 2) printf("%-30s ", *(char**)p);
		}
	}
	printf("\n");
}

void PrintColumns(t_table *T)
{
	//primeste un pointer la un tabel si afiseaza numele tabelului si coloanele
	printf("TABLE: %s\n", T->name);
	if (T->columns)
	{
		int i, nr;
		t_column *p;
		for (p = T->columns, nr = 1; p != NULL; p = p->next, nr++)
			printf("%-30s ", p->name);
		printf("\n");

		for (i = 1; i < nr; i++)
			printf("%s ", PADDING);
		printf("\n");
	}
}

void PrintDB(t_db *DB)
{
	//primeste un pointer la DB si afiseaza toate tabelele
	printf("DATABASE: %s\n\n", DB->name);
	if (DB->tables)
	{
		t_table *p;
		for (p = DB->tables; p != NULL; p = p->next)
		{
			PrintColumns(p);
			if (p->lines)
			{
				void *q;
				for (q = p->lines; q != NULL; q = *(char**)(q+sizeof(void*)))
					PrintLine(q, p->type);
			}
			printf("\n");
		}
	}
}

int PrintTable(t_db *DB, char *TABLE_NAME)
{
	//primeste numele unui tabel si il afiseaza
	if (DB->tables)
	{
		t_table *p;
		for (p = DB->tables; p != NULL; p = p->next)
		{
			if (!strcmp(p->name, TABLE_NAME))
			{
				PrintColumns(p);
				if (p->lines)
				{
					void *q;
					for (q = p->lines; q != NULL; q = *(char**)(q+sizeof(void*)))
						PrintLine(q, p->type);
				}
				printf("\n");
				return 1;
			}
		}
	}
	return 0;
}
