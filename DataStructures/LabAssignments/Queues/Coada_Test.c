#include "TCoada.h"
#include "TStiva.h"
#include "utils.h"


void afisInt(void* a) {
	printf("%d", *(int*)a);
}

int cmpInt(void *a, void *b) {
	return *(int*)a - *(int*)b;
}

/* TODO EX 2 A */
void MarkIsland(Coordinate pos, int value) {
	TCoada q = InitQ();
	int i, j;
	Coordinate *new, *pct;
	new = malloc(sizeof(Coordinate));
	new->x = pos.x;
	new->y = pos.y;
	Enqueue(q, new);
	while(Peek(q) != NULL)
	{
		pct = Dequeue(q);
	 	map[pct->x][pct->y] = value;
		for (i = 0; i < 3; i++)
		{		
			for (j = 0; j < 3; j++)
			{
				if (	
					(pct->x+dirX[i] >= 0 && pct->x+dirX[i] <= 9) &&
					(pct->y+dirY[j] >= 0 && pct->y+dirY[j] <= 9) &&
					(map[pct->x+dirX[i]][pct->y+dirY[j]] == 9)
				   )	
				{
					new = malloc(sizeof(Coordinate));
					new->x = pct->x+dirX[i];
					new->y = pct->y+dirY[j];
					Enqueue(q, new);
				}
			}
		}
		free(pct);
	}
	free(q);
}

/* parcurge harta si in cazul in care intalneste o insula o marcheaza */
void NumberOfIslands() {
	int i, j;
	int count = 0;
	Coordinate pos;

	printf("Searching for islands...\n\n");

	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			if(map[i][j] == P) {
				count++;
				pos.x = i;
				pos.y = j;
				MarkIsland(pos, count);
			}
		}
	}

	printf("Found %d islands\n\n", count);
}

/* TODO EX 2 B */
void FloodFill(Coordinate pos, int new_color) \
{	
	if (map[pos.x][pos.y] != 9)
	{
		TCoada q = InitQ();
		int i, j;
		Coordinate *new, *pct;
		new = malloc(sizeof(Coordinate));
		new->x = pos.x;
		new->y = pos.y;
		Enqueue(q, new);
		while(Peek(q) != NULL)
		{
			pct = Dequeue(q);
		 	map[pct->x][pct->y] = new_color;
			for (i = 0; i < 3; i++)
			{		
				for (j = 0; j < 3; j++)
				{
					if (	
						(pct->x+dirX[i] >= 0 && pct->x+dirX[i] <= 9) &&
						(pct->y+dirY[j] >= 0 && pct->y+dirY[j] <= 9) &&
						(map[pct->x+dirX[i]][pct->y+dirY[j]] == 0)
					   )	
					{
						new = malloc(sizeof(Coordinate));
						new->x = pct->x+dirX[i];
						new->y = pct->y+dirY[j];
						Enqueue(q, new);
					}
				}
			}
			free(pct);
		}
		free(q);
	}
}

/*
	apeleaza FloodFill pornind de la un punct aleator si
	folosind o culoare aleatoare
*/
void RandomFloodFill() {
	Coordinate pos;
	pos.x = rand() % N;
	pos.y = rand() % N;
	int value = rand() % 9 + 1;

	printf("Coloring from point (%d,%d) with color %d\n\n", pos.x, pos.y, value);
	FloodFill(pos, value);
}

/* TODO EX 2 C */
void SolveLabyrinth(Coordinate start, Coordinate finish) {	
	TCoada q = InitQ();
	int i, j;
	Coordinate *new, *pct;
	new = malloc(sizeof(Coordinate));
	new->x = start.x;
	new->y = start.y;
	Enqueue(q, new);
	while(Peek(q) != NULL)
	{
		pct = Dequeue(q);
		map[pct->x][pct->y] = 8;
		if (pct->x == finish.x && pct->y == finish.y)
		{
			free(pct);
			break;
		}
		for (i = 0; i < 3; i++)
		{
			for (j = 0; j < 3; j++)
			{
				if (
					(pct->x+dirX[i] >= 0 && pct->x+dirX[i] <= 9) &&
					(pct->y+dirY[j] >= 0 && pct->y+dirY[j] <= 9) &&
					(map[pct->x+dirX[i]][pct->y+dirY[j]] == 0)
				   )	
				{
					new = malloc(sizeof(Coordinate));
					new->x = pct->x+dirX[i];
					new->y = pct->y+dirY[j];
					Enqueue(q, new);
				}
			}
		}
		free(pct);
	}
	while(Peek(q) != NULL)
	{
		pct = Dequeue(q);
		free(pct);
	}
	free(q);
}

/* apeleaza SolveLabyrinth cu coordonate aleatoare */
void RandomLabyrinth() {
	Coordinate start;
	start.x = rand() % N;
	start.y = rand() % N;

	while(map[start.x][start.y] != 0) {
		start.x = rand() % N;
		start.y = rand() % N;
	}
	
	Coordinate finish;
	finish.x = rand() % N;
	finish.y = rand() % N;

	while(map[finish.x][finish.y] != 0) {
		finish.x = rand() % N;
		finish.y = rand() % N;
	}

	printf("Searching a way from point (%d,%d) to point (%d,%d)\n\n", start.x, start.y, finish.x, finish.y);
	SolveLabyrinth(start, finish);
}

int main() {
	/* exemplu utilizare functii pe coada */
	TCoada q = InitQ();
	int vec[5] = {2,3,1,5,4};
	int *x;

	for (int i = 0; i < 5; i++) {
		// elementele puse in coada trebuie alocate pe heap 
		x = malloc(sizeof(int));
		*x = vec[i];
		Enqueue(q, x);
	}

	AfisareQ(q, afisInt);
		
//	afisInt(Peek(q));		// print 2
//	printf(" PEEK\n");
//	afisInt(Dequeue(q));	// print 2
//	printf(" DEQUEUE\n");
//	afisInt(Peek(q));		// print 3 (noul varf)
//	printf(" NEW PEEK\n");

	printf("Sortare coada: ");
	SortareQ(q, cmpInt);	// merge doar daca a fost implementata
	AfisareQ(q, afisInt);
	DistrQ(q);

	srand(time(NULL));
	printf("Initial map:\n\n");
	PrintMap();
	NumberOfIslands();
	PrintMap();
	
	ResetMap();
	RandomFloodFill();	
	PrintMap();
		
	ResetMap();
	RandomLabyrinth();
	PrintMap();
	

	/* apeluri functii de prelucrare harta */

	return 1;
}
