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

#ifndef _STIVA_H_
#define _STIVA_H_

/* FUNCTII STIVA */


/**
 * Initializare Stiva goala: varful stivei este pointer la
 * un TElem = NULL.
 */
TStiva InitS();

/**
 * Inserare in varful stivei. Daca nu se poate aloca spatiu pentru
 * un nou element, functia da return fara sa adauge nimic in stiva.
 * 
 * @param	s		referinta catre stiva in care se insereaza
 * @param 	x		referinta catre structura/zona de memorie inserata
 */
void Push(TStiva s, void *x);

/**
 * Elimina un element din varful stivei.
 * 
 * @param 	s		referinta catre stiva din care se sterge
 * @return			referinta catre informatia din elementul eliminat/NULL
 */
void *Pop(TStiva s);

/**
 * Verifica elementul din varful stivei.
 * (care va fi scos la urmatorul apel Pop)
 * 
 * @param	s		referinta catre stiva verificata
 * @return			referinta catre informatia din elementul din varf/NULL
 */
void *Top(TStiva s);

/**
 * Scoate un element din @param source si il pune in @param dest
 */
void MutaS(TStiva dest, TStiva source);

/**
 * @param	s		referinta catre stiva
 * @return			numarul de elemente din stiva primita ca parametru
 */
int NrES(TStiva s);

/**
 * Afiseaza elementele stivei in format human-readable, fara a parcurge stiva,
 * ci prin utilizarea unei stive secundare in care se pun elementele
 * pe masura ce sunt scoase, urmand a fi puse inapoi in stiva initiala.
 * 
 * @param	s			referinta catre stiva afisata
 * @param	afiEl		referinta catre o functie care afiseaza continutul
 *						unui element
 */
void AfisareS(TStiva s, void (*afiEl)(void *));

/**
 * Elibereaza memoria pentru intreaga stiva, si o sterge din memorie.
 * (stiva devine inutilizabila)
 * 
 * @param	s			referinta catre stiva ce va fi distrusa (va deveni NULL)
 */
void DistrS(TStiva s);

//Tema de casa
void InverseazaS(TStiva s);

//BONUS
void SortareS(TStiva s, TFCmp cmp);

#endif /* _STIVA_H_ */
