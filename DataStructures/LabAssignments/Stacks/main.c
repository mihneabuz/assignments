#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "TCoada.h"
#include "TStiva.h"

/* numar maxim elemente coada / stiva */
#define MAX 256

typedef struct
{
    int x, y;
} TPct;

typedef struct
{
    int id;
    int diametru;
} TDisc;

typedef struct
{
    int id;
    int varsta;
} TPersoana;

int TComp(TPct *a, void*b)
{
	return (a->y - *(int*)b);
}

int TPar(TPct *a)
{
	return !(a->y%2);
}

int GenereazaCoadaPct(void* c, size_t N, int v1, int v2)
{
    int n = 0;
    TPct p;

    srand(time(NULL));
    for(; N > 0; N--)
    {
        p.x = rand() % (v2-v1+1) + v1;
        p.y = rand() % (v2-v1+1) + v1;

        if (!IntrQ(c, &p))
            return n;
        n++;
    }
    return n;
}

int GenereazaStivaPct(void* c, size_t N, int v1, int v2)
{
    int n = 0;
    TPct p;

    srand(time(NULL));
    for(; N > 0; N--)
    {
        p.x = rand() % (v2-v1+1) + v1;
        p.y = rand() % (v2-v1+1) + v1;

        if (!Push(c, &p))
            return n;
        n++;
    }
    return n;
}

/* afiseaza coada */
void AfiQ(char antet[], TF1 AfiElem, void *a)
{
    /* afiseaza antet si apoi aplica functia de afisare AfiElem tuturor elementelor */
    printf ("%s", antet);          	/* afiseaza antetul */
    if (VidaQ (a))
        printf ("Coada vida!");
    else
        PrelQ (a, AfiElem);       	/* afiseaza elementele in ordinea introducerii */
    printf("\n");
}

/* afiseaza stiva */
void AfiS(char antet[], TF1 AfiElem, void *a)
{
    /* afiseaza antet si apoi aplica functia de afisare AfiElem tuturor elementelor */
    printf ("%s", antet);          /* afiseaza antetul */
    if (VidaS (a))
        printf ("Stiva vida!");
    else
        Prel_B_V (a, AfiElem);       /* afiseaza elementele in ordinea introducerii */
    printf("\n");
}

int AfiPct(void *a)
{
    printf("[%d %d] ", (*(TPct*)a).x, (*(TPct*)a).y);
    return 1;
}

/**
 * parametru: coada
 * rezultat: o stiva nou creata, in care copiaza elementele din coada
 * (elementul de la varful stivei trebuie sa fie primul element din coada)
 */
void *Copie(void *aQ)
{
    // Initializam o stiva ale carei elemente au aceeasi dimensiune cu elementele din coada - DimEQ(ac)
    // iar numarul maxim de elemente din stiva este egal cu numarul de elemente din coada - NrEQ(ac)
    void *auxS = InitS(DimEQ(aQ), NrEQ(aQ));

    // Folosim o coada auxiliara ca sa restauram coada initiala
    void *auxQ = InitQ(DimEQ(aQ), NrEQ(aQ));

    while (!VidaQ(aQ)) {
        void *el = malloc(sizeof(DimEQ(aQ)));

        ExtrQ(aQ, el);
        Push(auxS, el);
        IntrQ(auxQ, el);

        free(el);
    }

    while (!VidaQ(auxQ)) {
        void *el = malloc(sizeof(DimEQ(aQ)));

        ExtrQ(auxQ, el);
        IntrQ(aQ, el);

        free(el);
    }

    // Odata obtinuta stiva cu elementele din coada, trebuie rasturnata
    void *aS = InitS(DimES(auxS), NrES(auxS));
    Rastoarna(aS, auxS);

    return aS;
}


float FC1(void *c)
{
	TPersoana *el = malloc(sizeof(TPct));
	void *cop = InitQ(DimEQ(c), MAX);
	float nrt = 0, nrf = 0;

	while(!VidaQ(c))
	{
		ExtrQ(c, el);
		if (el->varsta%2 == 0)
			nrf++;
		nrt++;
		IntrQ(cop, el);
	}
	ConcatQ(c, cop);
	free(el);
	return (nrf/nrt)*100;
}
float FS1(void *s)
{
	TDisc *el = malloc(sizeof(TPct));
	void *cop = InitS(DimES(s), MAX);
	float nrt = 0, nrf = 0;

	while(!VidaS(s))
	{
		Pop(s, el);
		if (el->diametru%2 == 0)
			nrf++;
		nrt++;
		Push(cop, el);
	}
	Rastoarna(s, cop);
	free(el);
	return (nrf/nrt)*100;
}

void FC2(void *c, int (*cmp)(TPct*, void*), void* v)
{
	TPct *el = malloc(sizeof(TPct));
	void *cop = InitQ(DimEQ(c), MAX);

	while(!VidaQ(c))
	{
		ExtrQ(c, el);
		if (cmp(el, v) >= 0)
			IntrQ(cop, el);
	}
	ConcatQ(c, cop);
	free(el);
}

void FS2(void *s, int (*cmp)(TPct*, void*), void* d)
{
	TPct *el = malloc(sizeof(TPct));
	void *cop = InitS(DimES(s), MAX);

	while(!VidaS(s))
	{
		Pop(s, el);
		if (cmp(el, d) <= 0)
			Push(cop, el);
	}
	Rastoarna(s, cop);
	free(el);
}

void *FC3(void *c, int (*f)(TPct*))
{
	TPct *el = malloc(sizeof(TPct));
	void *cop = InitQ(DimEQ(c), MAX);
	void *imp = InitQ(DimEQ(c), MAX);
	
	while(!VidaQ(c))
	{
		ExtrQ(c, el);
		if (f(el))
			IntrQ(cop, el);
		else
			IntrQ(imp, el);
	}
	ConcatQ(c, cop);
	free(el);
	return imp;
}

void *FS3(void *s, int (*f)(TPct*))
{
	TPct *el = malloc(sizeof(TPct));
	void *cop = InitS(DimES(s), MAX);
	void *imp = InitS(DimES(s), MAX);

	Rastoarna(cop, s);
	while(!VidaS(cop))
	{
		Pop(cop, el);
		if (f(el))
			Push(s, el);
		else
			Push(imp, el);
	}
	free(el);
	return imp;
}
int main()
{
    void *c, *s;
    int lgc, lgs;

    c = InitQ(sizeof(TPct), MAX);
    if (!c)
        return 1;

    s = InitS(sizeof(TPct), MAX);
    if (!s)
    {
        DistrQ(&c);
        return 1;
    }

    lgc = GenereazaCoadaPct(c, 8, -5, 5);
    lgs = GenereazaStivaPct(s, 6, 8, 14);

    // v si d pentru ex 2
    int v = 1; 
    int d = 12;

    printf("\nCoada cu %d elemente\n", lgc);
    AfiQ("Elemente coada:\n", AfiPct, c);
    printf("1) Procentul este: %f \n", FC1(c));
    FC2(c, TComp, &v);
    AfiQ("2) Elimina din coada elementele < 1: ", AfiPct, c);
    AfiQ("3) Elemente impare: ", AfiPct, FC3(c, TPar));
    AfiQ("   Elemente   pare: ", AfiPct, c);    

    printf("\n\nStiva cu %d elemente\n", lgs);
    AfiS("Elemente stiva (baza -> varf):\n", AfiPct, s);
    printf("1) Procentul este: %f \n", FS1(s));
    FS2(s, TComp, &d);
    AfiS("2) Elimina din stiva elementele > 12: ", AfiPct, s);
    AfiS("3) Elemente impare: ", AfiPct, FS3(s, TPar));
    AfiS("   Elemente   pare: ", AfiPct, s);

    /* apeluri functii */
#if 0
    printf("\n--##--Tema de casa:\n");

    // Generam o coada de puncte
    void *aQ = InitQ(sizeof(TPct), MAX);
    lgc = GenereazaCoadaPct(aQ, 8, -5, 5);

    // Copied coada intr-o stiva
    void *aS = Copie(aQ);
    lgs = NrES(aS);

    printf("\nCoada cu %d elemente\n", lgc);
    AfiQ("Elemente coada:\n", AfiPct, aQ);
    printf("\n\nStiva cu %d elemente\n", lgs);
    AfiS("Elemente stiva (baza -> varf):\n", AfiPct, aS);

#endif // 1

    DistrS(&s);
    DistrQ(&c);
    return 0;
}
