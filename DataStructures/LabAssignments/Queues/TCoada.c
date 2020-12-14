#include "TCoada.h"
#include "TStiva.h"

/* FUNCTII COADA */


/* initiaza coada goala -> coada goala */
TCoada InitQ() {
	TCoada new = (TCoada)malloc(sizeof(TElem));
	if (!new) return NULL;
	*new = NULL;

	return new;
}

/* adauga elemtul x la sfarsitul cozii */
void Enqueue(TCoada q, void *x) {
	TElem new = (TElem)malloc(sizeof(TCelula));
	if (!new) return;
	new->info = x;
	new->urm = NULL;

	if(Vida(q)) {
		*q = new;
		return;
	}

	TElem aux = *q;

	while(aux->urm != NULL)
		aux = aux->urm;

	aux->urm = new;
}

/* scoate un element din varful cozii -> referinta catre continutul elementului eliminat */
void *Dequeue(TCoada q) {
	if(Vida(q))
		return NULL;
	
	void *result = (*q)->info;
	TElem aux = *q;
	*q = (*q)->urm;
	free(aux);

	return result;
}

/* -> referinta catre continutul elementului din varful cozii */
void *Peek(TCoada q) {
	if (Vida(q))
		return NULL;
	
	return (*q)->info;
}

/* muta din source in dest un element */
void MutaQ(TCoada dest, TCoada source) {
	Enqueue(dest, Dequeue(source));
}

/* -> numarul de elemente din stiva */
int NrEQ(TCoada q) {
	TCoada aux = InitQ();
	int count = 0;

	while (!Vida(q)) {
		MutaQ(aux, q);
		count++;
	}

	while(!Vida(aux))
		MutaQ(q, aux);

	return count;	
}

void ConcatQ(TCoada dest, TCoada source) {
	while(!Vida(source))
		MutaQ(dest, source);
}

/* afiseaza elementele stivei fara a itera prin structura */
void AfisareQ(TCoada q, void (*afiEl)(void *)) {
	TCoada aux = InitQ();

	printf("[");
	while (!Vida(q)) {
		void *elm = Dequeue(q);
		afiEl(elm);
		if (!Vida(q))
			printf(", ");
		Enqueue(aux, elm);
	}
	printf("]\n");

	while(!Vida(aux)) {
		Enqueue(q, Dequeue(aux));
	}

	DistrQ(aux);
}

void DistrQ(TCoada q) {
	while (!Vida(q))
		free(Dequeue(q));

	free(q);
	q = NULL;
}

//Tema de casa
void InverseazaQ(TCoada q) {}

void afisInt(void *a);

/* TODO BONUS -> sortare coada*/
void SortareQ(TCoada q, TFCmp cmp)
{
	int card = NrEQ(q);
	TStiva s1, s2;
	s1 = InitS();
	s2 = InitS();
	void *el, *min;
	el = Dequeue(q);
	min = el;
	Push(s1, el);
	while(Peek(q) != NULL)
	{
		el = Dequeue(q);
		if (cmp(el, min) < 0) min = el;
		Push(s1, el);
	}
	while(NrEQ(q) < card)
	{
		while(Top(s1) != NULL)
		{
			el = Pop(s1);
			if (cmp(el, min) == 0) Enqueue(q, el);
			else Push(s2, el);
		}
		min = Pop(s2);
		Push(s1, min);
		while(Top(s2) != NULL)
		{
			el = Pop(s2);
			if (cmp(el, min) < 0) min = el;
			Push(s1, el);
		}
	}
	free(s1);
	free(s2);
}
