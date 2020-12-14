#include "TArb.h"

static char desen[18][81], cri[] = "xxx";  /* zona reprezentare arbore, info */
static int limita = 17;                    /* linia limita de desenare */

void Repr(TArb r, int centru, int linie, int dif)
		 /*- functie auxiliara - pregateste desen arbore, cu radacina in centru linie+1
	 dif - distanta pe orizontala intre r si fiecare dintre cei doi descendenti
	 Exemple:  dif = 5    |        dif = 2    |
					 xxx                 xxx
				___/ \___              / \
			 |         |            |   |                               -*/
{
	int k;

	if (linie > limita) return;
	if (dif <= 1) {      /* spatiu de afisare insuficient -> ... */
		memset(desen[linie]+centru, '.', 3);
		return;
	}
	desen[linie][centru] = '|';      /* legatura spre nod */
	desen[linie][0] = ':';
	if (linie+1 > limita) return;
	sprintf(cri, "%3i", r->info);   /* converteste valoare info la 3 caractere */
	k = (cri[0] == ' ') ? 2 : 1;    /* decalaj fata de centru */
	memcpy(desen[linie+1] + centru - k, cri, 3); /* copiaza cele 3 caractere in desen */
	desen[linie+1][0] = ':';
	if (linie+2 > limita) return;
	if (r->st) {
		desen[linie+2][centru-1] = '/';                     /* reprezinta ramura  ____/  */
		desen[linie+2][0] = ':'; desen[linie+3][0] = ':';
		memset(desen[linie+2] + centru - dif + 1, '_', dif-2);
		Repr(r->st, centru - dif, linie + 3, (dif+1)/2 );   /* si subarborele stang */
	}
	if (r->dr) {
		desen[linie + 2][centru+1] = '\\';                  /* reprezinta ramura  \____  */
		desen[linie+2][0] = ':'; desen[linie+3][0] = ':';
		memset (desen[linie+2] + centru + 2, '_', dif-2);
		Repr(r->dr, centru + dif, linie + 3, (dif+1)/2 );   /* si subarborele drept */
	}
}

void AfisareArb(TArb r) /*- afiseaza arborele r -*/
{
	int j;

	for (j = 0; j <= limita; j++)
		memset (desen[j], ' ', 79);
	if (!r)
		printf ("%49s", "-=- Arbore VID -=-");
	else {
		Repr(r,40,0,19); /* preg reprezentare cu centrul in coloana 39 a primei linii */
		for (j = 0; j <= limita && desen[j][0] == ':'; j++) {
			desen[j][79] = '\0'; printf("%s\n", desen[j]+1);
		}
	}
	printf ("\n");
}

int NumaraMultiplu(TArb a)
{
	if (a == NULL) return 0;
	if (a->info % 3 == 0)
		return NumaraMultiplu(a->dr) + NumaraMultiplu(a->st) + 1;
	else
		return NumaraMultiplu(a->dr) + NumaraMultiplu(a->st);
}

int sterge(TArb a)
{
	if (a == NULL)
 		return 0;
	if (a->info % 2 == 0)
	{
		distruge(a);
		return 1;
	}
	else
	{
		if (sterge(a->st))
			a->st = NULL;
		if (sterge(a->dr))
			a->dr = NULL;
		return 0;
	}
}

void StergePare(TArb a)
{
	if (a == NULL)
		return;
	if (sterge(a->st))
		a->st = NULL;
	if (sterge(a->dr))
		a->dr = NULL;
}

void AfiseazaPare(TArb a)
{
	if (a != NULL)
	{
		if ( a->st != NULL )
		{
			AfiseazaPare(a->st->st);
			AfiseazaPare(a->st->dr);
		}
		if ( a->dr != NULL )
		{
	 		AfiseazaPare(a->dr->st);
			AfiseazaPare(a->dr->dr);
		}
		printf("%d ", a->info);
	}
}
int findNodes(TArb a, int n1, int n2)
{
	//verifica daca exista n1 si n2 in arborele a
	if (a == NULL) return 0;
	if (a->info == n1 || a->info == n2) 
		return 1 + findNodes(a->st, n1, n2) + findNodes(a->dr, n1, n2);
	return findNodes(a->st, n1, n2) + findNodes(a->dr, n1, n2);
}

TArb findLCA(TArb a, int n1, int n2)
{
	//gaseste LCA lui n1 si n2, plecand de la presupunerea ca
	//n1 si n2 exista in a
	if (a == NULL) return NULL;
	if (a->info == n1 || a->info == n2)	return a;

	TArb LCA_st = findLCA(a->st, n1, n2);
	TArb LCA_dr = findLCA(a->dr, n1, n2);

	if (LCA_st && LCA_dr) return a;
	if (LCA_st != NULL) return LCA_st;
	else return LCA_dr;
}

TArb LCA(TArb a, int n1, int n2)
{
	if (a == NULL) return NULL;
	if (findNodes(a, n1, n2) == 2)
		return findLCA(a, n1, n2);
	return NULL;
}

int main ()
{
	TArb arb;
	randomize();

    arb = create_ordered_tree(6);
	printf("_______Arbore 1________\n");
    AfisareArb(arb);
	printf("Ex1: Nr noduri cu informatie multiplu de 3: %d\n", NumaraMultiplu(arb));
	printf("Ex2: Informatiile de pe niveluri pare in postordine: ");	
	AfiseazaPare(arb);
	printf("\n");
	printf("Ex3: In urma stergerii nodurilor cu informatie pare, arborele devine:\n");
	StergePare(arb);
	AfisareArb(arb);
    distrugeArb(&arb);

    arb = create_random_tree(8);
	printf("_______Arbore 2________\n");
    AfisareArb(arb);
	printf("Ex1: Nr noduri cu informatie multiplu de 3: %d\n", NumaraMultiplu(arb));
	printf("Ex2: Informatiile de pe niveluri pare in postordine: ");	
	AfiseazaPare(arb);
	printf("\n");
	printf("Ex3: In urma stergerii nodurilor cu informatie pare, arborele devine:\n");
	StergePare(arb);
	AfisareArb(arb);
    distrugeArb(&arb);

    arb = create_ordered_tree(8);
	printf("_______Arbore 3________\n");
    AfisareArb(arb);
	printf("Ex1: Nr noduri cu informatie multiplu de 3: %d\n", NumaraMultiplu(arb));
	printf("Ex2: Informatiile de pe niveluri pare in postordine: ");	
	AfiseazaPare(arb);
	printf("\n");
	printf("BONUS: ");
	int nod1, nod2;
	nod1 = 7;
	nod2 = 2;
	TArb aux = LCA(arb, nod1, nod2);
	if (aux == NULL) 
		printf("Nodurile %d si/sau %d nu au fost gasite in arbore\n", nod1, nod2);
	else
		printf("Primul stramos comun al nodurilor %d si %d este: %d\n", nod1, nod2, aux->info);
	printf("Ex3: In urma stergerii nodurilor cu informatie pare, arborele devine:\n");
	StergePare(arb);
	AfisareArb(arb);
    distrugeArb(&arb);

	return 0;
}
