/*--- graf-M.c --- Graf implementat cu Matrice de adiacente --*/
#include <string.h>
#include "graf-M.h"

void *AlocG(int nr)
{
    int i;
    AGM g = (AGM)malloc(sizeof(TGM)); /* spatiu descriptor */
    if (!g)
        return NULL;
    g->x = (int *)malloc(nr * nr * sizeof(int)); /* matrice liniarizata */
    if (!g->x)
    {
        free(g);
        return NULL;
    }
    for (i = 0; i < nr * nr; i++)
        g->x[i] = ArcInex;
    g->nn = nr;
    return (void *)g;
}

void DistrG(void **ag)
{
    free(((AGM)(*ag))->x);
    free(*ag);
    *ag = NULL;
}

int CostArc(void *g, int s, int d)
{
    /* functia intoarce costul arcului (s, d); daca arcul nu exista intoarce ArcInex */
    AGM gm = (AGM)g;
    int cost = gm->x[(s-1) * gm->nn + d - 1];
    if (cost)
        return cost;
    return ArcInex;
}

int NrNoduri(void *g)
{
    return ((AGM)g)->nn;
}

int CitGraf(void **ag)
{
    FILE *f;
    char numefis[21];
    int i, j, n;
    int x;
    AGM g;

    printf("Numele fisierului de descriere a grafului = ");
    if (!scanf("%s", numefis))
        return -1;
    f = fopen(numefis, "rt");
    if (!f)
        return -2;
    if (!fscanf(f, "%i", &n) || n <= 0)
        return -3;
    *ag = g = (AGM)AlocG(n);
    if (!g)
        return 0;
    while (fscanf(f, "%i%i%i", &i, &j, &x) == 3)
    {
        if (i < 1 || i > n || j < 1 || j > n)
        {
            fclose(f);
            return -4;
        }
        ((AGM)g)->x[(i - 1) * ((AGM)g)->nn + j - 1] = x;
    }
    fclose(f);
    return 1;
}

int gradIntrare(void *g, int nod) {
	int i, n, grad = 0;
	n = NrNoduri(g);
	for (i = nod-1; i <= n * n; i += n)
		if (((AGM)g)->x[i] != 0) grad++;
	return grad;
}

void BFS(void *g, int nod) {
	int x[((AGM)g)->nn + 1];
	int vizitat[((AGM)g)->nn + 1];
	x[0] = nod;
	int start = 0, stop = 1, curent, i, n = ((AGM)g)->nn;
	for (i= 1; i <= n; i++)
		vizitat[i] = 0;
	while(start < stop)
	{
		curent = x[start];
		start++;
		printf("%d ", curent);
		vizitat[curent] = 1;
		for (i = 0; i < n; i++)
			if (((AGM)g)->x[(curent-1)*n + i] != 0 && vizitat[i+1] != 1)
			{
				vizitat[i+1] = 1;
				x[stop] = i+1;
				stop++;
			}
	}
	printf("\n");
}

int ArcCostMin(void *g) {
	int i, min, n = ((AGM)g)->nn;

	for (i = 0; i < n*n; i++)
		if (((AGM)g)->x[i] != 0)
			if (min == 0 || ((AGM)g)->x[i] < min) min = ((AGM)g)->x[i];
	return min;
}
