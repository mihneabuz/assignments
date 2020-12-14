/*--- test-graf.c ---*/
#include "graf.h"

int gradIntrare(void *g, int nod);	
void BFS(void *g, int nod);
int ArcCostMin(void *g);

void AfiM(void *g)
{
    int i, j, k, n = NrNoduri(g);
    for (i = 1; i <= n; i++)
    {
        for (j = 1; j <= n; j++)
        {
            k = CostArc(g, i, j);
            if (k == ArcInex)
                printf("  - ");
            else
                printf("%3i ", k);
        }
        printf("\n");
    }
}

int main()
{
    void *g;

    if (CitGraf(&g) <= 0)
    {
        printf("-=- eroare la citire -=-\n");
        return -1;
    }
    printf(" -=- graf cu %i noduri -=-\n", NrNoduri(g));
    AfiM(g);
    /*-- apel functii definite --*/

	int nod = 1;
	printf("Gradul de intrare al nodului %d este %d\n", 
			nod, 
			gradIntrare(g, nod));

	printf("Parcurgere in latime din nodul %d:\n", nod);
	BFS(g, nod);
	printf("Arcul de cost minim are valoarea: %d\n", ArcCostMin(g));

    DistrG(&g);
    return 0;
}
