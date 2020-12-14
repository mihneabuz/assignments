#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>


#ifndef _ELEMENT_
#define _ELEMENT_

/* -> T/F daca Stiva/Coada nu contine elemente */
#define Vida(T) ((*T) == NULL)
/* tipul functiei de comparare folosita pentru sortari */
typedef int (*TFCmp)(void*, void*);

typedef struct celula
{
	void *info;							/* adresa element extern */
	struct celula *urm;					/* legatura catre urmatorul element */
} TCelula, *TElem, **TStiva, **TCoada;

#endif /* _ELEMENT_ */


#ifndef _COADA_H_
#define _COADA_H_

/* FUNCTII COADA */


/**
 * Initializare Coada goala: varful cozii este pointer la
 * un TElem = NULL.
 */
TCoada InitQ();

/**
 * Inserare la sfarsitul cozii. Daca nu se poate aloca spatiu pentru
 * un nou element, functia da return fara sa adauge nimic in coada.
 * 
 * @param	q		referinta catre coada in care se insereaza
 * @param 	x		referinta catre structura/zona de memorie inserata
 */
void Enqueue(TCoada q, void *x);

/**
 * Elimina un element din varful cozii.
 * 
 * @param 	q		referinta catre coada din care se sterge
 * @return			referinta catre informatia din elementul eliminat/NULL
 */
void *Dequeue(TCoada q);

/**
 * Verifica elementul din varful cozii
 * (care va fi scos la urmatorul apel Dequeue)
 * 
 * @param q			referinta catre coada verificata
 * @return			referinta catre informatia din elementul din varf/NULL
 */
void *Peek(TCoada q);

/** Scoate un element din @param source si il pune in @param dest */
void MutaQ(TCoada dest, TCoada source);

/**
 * @param	s		referinta catre stiva
 * @return			numarul de elemente din stiva primita ca parametru
 */
int NrEQ(TCoada q);

/** Muta toate elementele din @param source in @param dest */
void ConcatQ(TCoada dest, TCoada source);

/**
 * Afiseaza elementele cozii in format human-readable, fara a parcurge coada,
 * ci prin utilizarea unei cozi secundare in care se pun elementele
 * pe masura ce sunt scoase, urmand a fi puse inapoi in coada initiala.
 * 
 * @param	q			referinta catre coada de afisat
 * @param	afiEl		referinta catre o functie care afiseaza continutul
 *						unui element
 */
void AfisareQ(TCoada q, void (*afiEl)(void *));

/**
 * Elibereaza memoria pentru intreaga coada, si o sterge din memorie.
 * (coada devine inutilizabila)
 * 
 * @param	q			referinta catre coada ce va fi distrusa (va deveni NULL)
 */
void DistrQ(TCoada q);

//Tema de casa
void InverseazaQ(TCoada q);

// BONUS
void SortareQ(TCoada q, TFCmp cmp);

#endif /* _COADA_H_ */
