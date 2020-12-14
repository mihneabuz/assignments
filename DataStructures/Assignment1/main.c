//Buzatu-Pahontu Mihnea-Nicolae
//315CB
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "structures.h"
#include "fPrint.h"
#include "fFree.c"
#include "fFunc.h"

void EndCMD()
{
	//citeste restul elementelor de pe line pentru a fi sigur ca nu au fost
	//date prea multe argumente unei comenzi
	//deasemenea citeste restul argumentelor comenzilor gresite
	char c;
	scanf("%c", &c);
	while (c != '\n') scanf("%c", &c);
}

void CREATE(t_db *DB)
{
	//comanda de CREATE <nume tabel> <tip date> [<nume coloana>]
	char *TABLE_NAME, *DATA_TYPE, *COLUMN, c;
	TABLE_NAME = malloc(MAX_TABLE_NAME_LEN * sizeof(char));
	DATA_TYPE = malloc(MAX_DATA_TYPE_LEN * sizeof(char));
	COLUMN = malloc(MAX_COLUMN_NAME_LEN * sizeof(char));
	scanf("%s %s%c", TABLE_NAME, DATA_TYPE, &c);
	if (getType(DATA_TYPE) != -1)
	{
		t_table *T = InitTable(TABLE_NAME, getType(DATA_TYPE));
		while(c != '\n')
		{
			scanf("%s%c", COLUMN, &c);
			AddColumn(T, COLUMN);
		}
		if (!AddTable(DB, T))
			printf("Table \"%s\" already exists.\n", TABLE_NAME);
	}
	else
	{
		printf("Unknown data type: %s\n", DATA_TYPE);
		EndCMD();
	}
	free(TABLE_NAME);
	free(DATA_TYPE);
	free(COLUMN);
}

void DELETE(t_db *DB)
{
	//comanda de DELETE <nume tabel> sau
	//DELETE <nume tabel> <nume coloana> <relatie> <valoare>
	char c, *TABLE_NAME = malloc(MAX_TABLE_NAME_LEN * sizeof(char));
	scanf("%s%c", TABLE_NAME, &c);
	if (c == '\n')
	{
		if (!DeleteTable(DB, TABLE_NAME))
			printf("Table \"%s\" not found in database.\n", TABLE_NAME);
	}
	else
	{
		char *COLUMN, *EXP;
		COLUMN = malloc(MAX_COLUMN_NAME_LEN * sizeof(char));
		EXP = malloc(MAX_EXP_LEN * sizeof(char));
		scanf("%s %s", COLUMN, EXP);
		if (getTable(DB, TABLE_NAME))
		{
			if (!EditLines(getTable(DB, TABLE_NAME), COLUMN, EXP, "DELETE"))
				printf("Table \"%s\" does not contain column \"%s\".\n", 
				TABLE_NAME,
				COLUMN);
		}
		else printf("Table \"%s\" not found in database.\n", TABLE_NAME);
		free(COLUMN);
		free(EXP);
		EndCMD();
	}
	free(TABLE_NAME);
}

void PRINT(t_db *DB)
{
	//comanda de PRINT <nume tabel>
	char *TABLE_NAME = malloc(MAX_TABLE_NAME_LEN * sizeof(char));
	scanf("%s", TABLE_NAME);
	if (!PrintTable(DB, TABLE_NAME))
		printf("Table \"%s\" not found in database.\n", TABLE_NAME);
	EndCMD();
	free(TABLE_NAME);
}

void SEARCH(t_db *DB)
{
	//comanda de SEARCH <nume tabel> <nume coloana> <relatie> <valoare>
	char *TABLE_NAME, *COLUMN, *EXP;
	TABLE_NAME = malloc(MAX_TABLE_NAME_LEN * sizeof(char));
	COLUMN = malloc(MAX_COLUMN_NAME_LEN * sizeof(char));
	EXP = malloc(MAX_EXP_LEN * sizeof(char));
	scanf("%s %s %s", TABLE_NAME, COLUMN, EXP);
	if (getTable(DB, TABLE_NAME))
	{
		if(!EditLines(getTable(DB, TABLE_NAME), COLUMN, EXP, "SEARCH"))
			printf("Table \"%s\" does not contain column \"%s\".\n",
			TABLE_NAME,
			COLUMN);
		else printf("\n");
	}
	else printf("Table \"%s\" not found in database.\n", TABLE_NAME);
	EndCMD();
	free(TABLE_NAME);
	free(COLUMN);
	free(EXP);
}
void ADD(t_db *DB)
{
	//comanda de ADD <nume tabel> [<valoare>]
	char *TABLE_NAME = malloc(MAX_TABLE_NAME_LEN * sizeof(char));
	scanf("%s", TABLE_NAME);
	if (getTable(DB, TABLE_NAME)) AddLine(getTable(DB, TABLE_NAME));
	else printf("Table \"%s\" not found in database.\n", TABLE_NAME);
	EndCMD();
	free(TABLE_NAME);
}

void CLEAR(t_db *DB)
{
	//comanda de CLEAR <nume tabel>
	char *TABLE_NAME = malloc(MAX_TABLE_NAME_LEN * sizeof(char));
	scanf("%s", TABLE_NAME);
	if (getTable(DB, TABLE_NAME))
	{
		FreeLines(getTable(DB, TABLE_NAME));
		getTable(DB, TABLE_NAME)->lines = NULL;
	}
	else printf("Table \"%s\" not found in database.\n", TABLE_NAME);
	EndCMD();
	free(TABLE_NAME);
}

int main()
{
	char *CMD = malloc(MAX_CMD_LEN * sizeof(char));
	t_db *DB;
	int run = 1;
	while (run)
	{
		scanf("%s", CMD);
		if (!strcmp(CMD, "INIT_DB"))
		{
			//comanda de INIT_DB <nume DB>
			char *DB_NAME = malloc(MAX_DB_NAME_LEN * sizeof(char));
			scanf("%s", DB_NAME);
			DB = malloc(sizeof(t_db));
			memcpy(DB->name, DB_NAME, MAX_DB_NAME_LEN);
			DB->tables = NULL;
			free(DB_NAME);
		}
		else if (!strcmp(CMD, "CREATE")) CREATE(DB);
		else if (!strcmp(CMD, "DELETE")) DELETE(DB);
		else if (!strcmp(CMD, "PRINT_DB")) PrintDB(DB);
		else if (!strcmp(CMD, "PRINT")) PRINT(DB);
		else if (!strcmp(CMD, "SEARCH")) SEARCH(DB);
		else if (!strcmp(CMD, "ADD")) ADD(DB);
		else if (!strcmp(CMD, "CLEAR")) CLEAR(DB);
		else if (!strcmp(CMD, "DELETE_DB"))
		{
			//comanda de DELETE_DB
			FreeDB(DB);
			run = 0;
		}
		else
		{
			//comanda necunoscuta -> eroare
			printf("Unknown command: %s", CMD);
			char c, *BUFFER = malloc(MAX_CMD_LEN);
			scanf("%c", &c);
			printf("%c", c);
			while(c != '\n')
			{
				scanf("%s%c", BUFFER, &c);
				printf("%s%c", BUFFER, c);
			}
			printf("%s\n", BUFFER);
		}
	}
	free(CMD);
	return 0;
}
