#include <time.h>

#ifndef _UTILS_H_
#define _UTILS_H_

#define N 10
#define P 9 // pamant | portocaliu | perete
#define W 0 //alb
#define R 1 //rosu
#define G 2 //verde
#define B 3 //albastru
#define Y 4 //galben
#define M 5 //mov
#define C 6 //cyan
#define Rz 7 //roz
#define L 8 //lime

typedef struct {
	int x, y;
} Coordinate;

int dirY[5] = {-1, 0, 1};
int dirX[5] = {-1, 0, 1};

int map[N][N]=
	{
		{ P, 0, P, 0, 0, 0, P, P, P, P },
		{ 0, 0, P, 0, P, 0, P, 0, 0, 0 },
		{ P, P, P, P, 0, 0, P, 0, 0, 0 },
		{ P, 0, 0, P, 0, P, 0, 0, 0, 0 },
		{ P, P, P, P, 0, 0, 0, P, P, P },
		{ 0, P, 0, P, 0, 0, P, P, P, P },
		{ 0, 0, 0, 0, 0, P, P, P, 0, 0 },
		{ 0, 0, 0, P, 0, 0, P, P, P, 0 },
		{ P, 0, P, 0, P, 0, 0, P, 0, 0 },
		{ P, P, P, P, 0, 0, 0, P, P, P }
	};

#endif /* _UTILS_H_ */

/* reincarca valorile hartii la varianta de mai sus */
void ResetMap() {
	FILE *f = fopen("map.info", "r");

	for(int i = 0; i < N; i++)
		for(int j = 0; j < N; j++)
			fscanf(f, "%d", &map[i][j]);
	
	fclose(f);
}

/* afiseaza harta folosind culori */
void PrintMap() {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			switch (map[i][j]) {
			case P:
				printf("\033[38;5;166m");
				break;
			
			case R:
				printf("\033[0;31m");
				break;

			case G:
				printf("\033[0;32m");
				break;
			
			case B:
				printf("\033[0;34m");
				break;
			
			case Y:
				printf("\033[0;33m");
				break;
			
			case M:
				printf("\033[0;35m");
				break;
			
			case C:
				printf("\033[0;36m");
				break;

			case Rz:
				printf("\033[0;38;5;205m");
				break;

			case L:
				printf("\033[0;38;5;154m");
				break;

			}
			printf("%d \033[0m", map[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}