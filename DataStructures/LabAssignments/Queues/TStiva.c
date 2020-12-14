#include "TStiva.h"
#include "TCoada.h"

/* FUNCTII STIVA */


/* initiaza stiva goala -> stiva goala */
TStiva InitS() {
	TStiva new = (TStiva)malloc(sizeof(TElem));
	if (!new) return NULL;
	*new = NULL;

	return new;
}

/* adauga elemtul x in varful stivei */
void Push(TStiva s, void *x) {
	TElem new = (TElem)malloc(sizeof(TCelula));
	if (!new) return; 
	new->info = x;
	new->urm = NULL;

	if(Vida(s)) {
		*s = new;
		return;
	}

	new->urm = *s;
	*s = new;
}

/* scoate un element din varful stivei -> referinta catre continutul elementului eliminat */
void *Pop(TStiva s) {
	if(Vida(s))
		return NULL;
	
	void *result = (*s)->info;
	TElem aux = *s;
	*s = (*s)->urm;
	free(aux);

	return result;
}

/* -> referinta catre continutul elementului din varful stivei */
void *Top(TStiva s) {
	if (Vida(s))
		return NULL;
	
	return (*s)->info;
}

/* muta din source in dest un element */
void MutaS(TStiva dest, TStiva source) {
	Push(dest, Pop(source));
}

/* -> numarul de elemente din stiva */
int NrES(TStiva s) {
	TStiva aux = InitS();
	int count = 0;

	while (!Vida(s)) {
		MutaS(aux, s);
		count++;
	}

	while(!Vida(aux))
		MutaS(s, aux);

	return count;	
}

/* afiseaza elementele stivei fara a itera prin structura */
void AfisareS(TStiva s, void (*afiEl)(void *)) {
	TStiva aux = InitS();

	printf("[");
	while (!Vida(s)) {
		void *elm = Pop(s);
		afiEl(elm);
		if (!Vida(s))
			printf(", ");
		Push(aux, elm);
	}
	printf("]\n");

	while(!Vida(aux))
		MutaS(s, aux);

	DistrS(aux);
}

/* elibereaza tot spatiul ocupat de stiva, inclusiv referinta catre varf */
void DistrS(TStiva s) {
	while (!Vida(s))
		free(Pop(s));

	free(s);
	s = NULL;
}

/* TODO Tema de casa */
void InverseazaS(TStiva s) {}

/* TODO BONUS -> sortare stiva */
void SortareS(TStiva s, TFCmp cmp) {}
