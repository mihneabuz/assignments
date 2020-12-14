//Buzatu-Pahontu Mihnea-Nicolae
//315CB

#include <stdlib.h>

typedef struct cell
{
	void *info;
	struct cell *next;
} TCell, *TList;
//Definitia listei generice

typedef struct stack
{
	size_t dim;
	TList top;
} tstack, *TStack;
//Definitia Stivei generice

typedef struct queue
{
	size_t dim;
	TList start, end;
} tqueue, *TQueue;
//Definitia Cozii generice

typedef struct page
{
	char *URL;
	int res_num;
	Resource *resources;
} PageElem, *TPage;
//Structura unei pagini

typedef struct tab
{
	TPage page;
	struct tab *next;
	TStack forward;
	TStack back;
} TElem, *TabStruct;
//Structura listei de taburi


TList NewCell(void *x); 
//aloca o celula nou cu informatia x si o returneaza

TPage NewPage(char *URL); 
//aloca o pagina noua cu URL si genereaza resursele

void FreePage(TPage page);
//elibereaza memoria paginii page

TabStruct NewTab();
//aloca o structura de tip tab noua si o returneaza

TabStruct AddNewTab(TabStruct *l);
//adauga un tab nou empty la lista de taburi l si returneaza adresa lui

void RemoveEnd(TabStruct *l);
//sterge ultimul tab din lista de taburi l si elibereaza memoria

TabStruct GetTab(TabStruct l, int ind);
//returneaza tabul cu indicele ind din lista de taburi l

void PrintTabInfo(TabStruct l, void f(char *x, int ind, FILE *out), FILE *out);
//printeaza toate elemntele listei de taburi l folosind functia f
//care stabileste formatul

void FreeTab(TabStruct l);
//elibereaza memoria de la structura de tab l

void FreeTabs(TabStruct l);
//Elibereaza memoria de la toate elementele liste de taburi l


//Functii pentru Coada
TQueue InitQ(size_t d);
//initializeaza o coada

int VoidQ(TQueue q);
//returneaza 1 daca coada q este vida si 0 daca are elemente

void Enqueue(TQueue q, void *x);
//introduce elementul x in coada q

void *Dequeue(TQueue q);
//extrage si returneaza primul element din coada

void *Peek(TQueue q);
//returneaza primul element al cozii q, fara sa il scoata din coada

void FreeQ(TQueue q);
//elibereaza toata memoria alocata cozii q

void PrintQueueInfo(TQueue q, void f(void *x, FILE *out), FILE *out);
//printeaza toate elementele cozii q, folosind functia f, in fisierul out
//f primeste un pointer spre un elemente si stabileste formatul de afisare

void InsertPriorityQ(TQueue q, void *x, int cmp(void *a, void *b));
//insereaza elementul x in coada q in pozitia corecta, sortata,
//folosind functia de compratie cmp care primeste 2 elemente


//Functii pentru Stiva
TStack InitS(size_t d);
//initializeaza o stiva

int VoidS(TStack s);
//returneaza 1 daca stiva e vida si 0 daca are elemente

void Push(TStack s, void *x);
//introduce elementul x in stiva s

void *Pop(TStack s);
//extrage si returneaza primul element din stiva s

void PrintStackInfo(TStack s, void f(void *x, FILE *out), FILE *out);
//printeaza toate elementele stivei s, folosind functia f, in fisierul out
//f primeste un pointer spre un element si stabileste formatul de afisare

void EmptyS(TStack s);
//elibereaza memoria de la toate elementele din stiva s
