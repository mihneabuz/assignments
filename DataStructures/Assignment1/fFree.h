//Buzatu-Pahontu Mihnea-Nicolae
//315CB
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structures.h"

//Sursa cu functiile de free

void FreeLineInt(t_intLine *Line)
{
	//elibereaza memoria unei linii Int
	t_intCell *p, *tmp;
	p = Line->cells;
	while(p != NULL)
	{
		tmp = p;
		p = p->next;
		free(tmp);
	}
	free(Line);
}

void FreeLineFloat(t_floatLine *Line)
{
	//elibereaza memoria unei linii Float
	t_floatCell *p, *tmp;
	p = Line->cells;
	while(p != NULL)
	{
		tmp = p;
		p = p->next;
		free(tmp);
	}
	free(Line);
}

void FreeLineString(t_stringLine *Line)
{
	//elibereaza memoria unei linii String
	t_stringCell *p, *tmp;
	p = Line->cells;
	while(p != NULL)
	{
		tmp = p;
		p = p->next;
		free(tmp->value);
		free(tmp);
	}
	free(Line);
}

void FreeLines(t_table *T)
{
	//elibereaza memoria din toate liniile tabelului T
	if (T->type == 0)
	{
		t_intLine *p, *tmp;
		p = T->lines;
		while(p != NULL)
		{
			tmp = p;
			p = p->next;
			FreeLineInt(tmp);
		}
	}
	if (T->type == 1)
	{
		t_floatLine *p, *tmp;
		p = T->lines;
		while(p != NULL)
		{
			tmp = p;
			p = p->next;
			FreeLineFloat(tmp);
		}
	}
	if (T->type == 2)
	{
		t_stringLine *p, *tmp;
		p = T->lines;
		while(p != NULL)
		{
			tmp = p;
			p = p->next;
			FreeLineString(tmp);
		}
	}
}

void FreeTable(t_table *T)
{
	//elibereaza memoria asociata tabelului T
	if (T->columns)
	{
		t_column *p, *tmp;
		p = T->columns;
		while(p != NULL)
		{
			tmp = p;
			p = p->next;
			free(tmp);
		}
		FreeLines(T);
	}
	free(T);
}

void FreeDB(t_db *DB)
{
	//elibereaza toata memoria asociata bazei de date
	if (DB->tables)
	{
		t_table *p, *tmp;
		p = DB->tables;
		while(p != NULL)
		{
			tmp = p;
			p = p->next;
			FreeTable(tmp);
		}
	}
	free(DB);
}
