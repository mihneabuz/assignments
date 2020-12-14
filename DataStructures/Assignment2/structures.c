//Buzatu-Pahontu Mihnea-Nicolae
//315CB

#include "structures.h"

TList NewCell(void *x)
{
	TList aux = (TList)malloc(sizeof(TCell));
	if (aux)
	{
		aux->info = x;
		aux->next = NULL;
	}
	return aux;
}

TPage NewPage(char *URL)
{
	TPage new = (TPage)malloc(sizeof(PageElem));
	if (new)
	{
		new->URL = malloc(sizeof(char)*(strlen(URL)+1));
		strcpy(new->URL, URL);
		new->resources = get_page_resources(URL, &new->res_num);
	}
	return new;
}

void FreePage(TPage page)
{
	free(page->URL);
	free(page->resources);
	free(page);	
}

TabStruct NewTab()
{
	TabStruct new = (TabStruct)malloc(sizeof(TElem));
	if (new)
	{
		new->page = NULL;
		new->next = NULL;
		new->forward = InitS(sizeof(TabStruct));
		new->back = InitS(sizeof(TabStruct));
	}
	return new;
}

TabStruct AddNewTab(TabStruct *l)
{
	TabStruct new = NewTab();
	if (!new) return NULL;
	
	if ((*l) == NULL)
	{
		(*l) = new;
		return *l;
	}

	TabStruct p = *l;
	while(p->next != NULL)
		p = p->next;
	p->next = new;
	return new;
}

void RemoveEnd(TabStruct *l)
{
	if (*l == NULL)	return;
	else if ((*l)->next == NULL)
	{
		free(*l);
		*l = NULL;
		return;
	}
	else
	{
		TabStruct p, aux;
		for (p = *l; p->next != NULL; aux = p, p = p->next);
		aux->next = NULL;
		FreeTab(p);
	}
}

TabStruct GetTab(TabStruct l, int ind)
{
	if (l == NULL) return NULL;
	if (ind == 0) return l;
	return GetTab(l->next, ind-1);
}

void PrintTabInfo(TabStruct l, void f(char *x, int ind, FILE *out), FILE *out)
{
	if (l == NULL) return;
	int ind;
	for (ind = 0 ; l != NULL ; l = l->next, ind++)
	{
		if (l->page) f(l->page->URL, ind, out);
		else
		{
			char aux[6] = "empty";
			f(aux, ind, out);
		}
	}
}

void FreeTab(TabStruct l)
{
	if (l->page) FreePage(l->page);
	TPage aux = Pop(l->back);
	while(aux != NULL)
	{
		FreePage(aux);
		aux = Pop(l->back);
	}
	free(l->back);
	aux = Pop(l->forward);
	while(aux != NULL)
	{
		FreePage(aux);
		aux = Pop(l->forward);
	}
	free(l->forward);
	free(l);
}

void FreeAllTabs(TabStruct l)
{
	TabStruct p, aux;
	if (l == NULL) return;
	p = l->next;
	aux = l;
	while(p != NULL)
	{
		FreeTab(aux);
		aux = p;
		p = p->next;
	}
	FreeTab(aux);
}

TQueue InitQ(size_t d)
{
	TQueue aux = (TQueue)malloc(sizeof(tqueue));
	if (!aux) return NULL;
	
	aux->dim = d;
	aux->start = NULL;
	aux->end = NULL;
	return aux;
}

int VoidQ(TQueue q)
{
	if (q->start == NULL && q->end == NULL)	return 1;
	return 0;
}

void Enqueue(TQueue q, void *x)
{
	//La introducere am adaugat direct pointerul spre elementul dorit
	//in loc sa copiez memoria de la adresa respectiva
	TList new = NewCell(x);
	if (!new) return;

	if (VoidQ(q))
	{
		q->start = new;
		q->end = new;
		return;
	}

	q->end->next = new;
	q->end = new;
}

void *Dequeue(TQueue q)
{
	if (VoidQ(q)) return NULL;

	void *x = q->start->info;
	TList aux = q->start;

	if (q->start == q->end)	q->start = q->end = NULL;
	else q->start = q->start->next;

	free(aux);
	return x;
}

void *Peek(TQueue q) {
	return q->start->info;
}

void FreeQ(TQueue q)
{
	while(!VoidQ(q))
		free(Dequeue(q));
	free(q);
}

void PrintQueueInfo(TQueue q, void f(void *x, FILE *out), FILE *out)
{
	TQueue aux = InitQ(q->dim);
	while(!VoidQ(q))
	{
		void *info = Dequeue(q);
		f(info, out);
		Enqueue(aux, info);
	}
	while(!VoidQ(aux)) Enqueue(q, Dequeue(aux));
	FreeQ(aux);
}

void InsertPriorityQ(TQueue q, void *x, int cmp(void *a, void *b))
{
	//Pentru a insera in ordine crescatoare, am iterat prin stiva pana am 
	//gasit un element mai mare decat x, si l-am inserat pe x inaintea lui
	TList new = NewCell(x);
	if (!new) return;

	//caz special daca coada este vida
	if (VoidQ(q))
	{
		q->start = new;
		q->end = new;
		return;
	}

	//caz special daca trebuie introdus pe prima pozitie
	if (cmp(q->start->info, new->info) > 0)
	{
		new->next = q->start;
		q->start = new;
		return;
	}

	TList aux = q->start, p = q->start->next;
	while( p && (cmp(p->info, new->info) < 0))
	{
		aux = p;
		p = p->next;
	}
	new->next = p;
	aux->next = new;
	if (aux == q->end) q->end = new;
}

TStack InitS(size_t d)
{
	TStack aux = (TStack)malloc(sizeof(tstack));
	if (!aux) return NULL;
	aux->dim = d;
	aux->top = NULL;
	return aux;
}

int VoidS(TStack s)
{
	if (s->top == NULL) return 1;
	else return 0;
}

void Push(TStack s, void *x)
{
	//La introducere, am adaugat direct pointerul spre element,
	//nu am mai copiat memoria de la adresa respectiva
	//deoarece principala utilizare a stivei este pentru 
	//forward si back, iar elementele acestor stive circula
	//intre forward->back->curent si nu este necesar sa fie copiate
	TList new = NewCell(x);
	if (!new) return;

	if (VoidS(s))
	{
		s->top = new;
		return;
	}

	new->next = s->top;
	s->top = new;
}

void *Pop(TStack s)
{
	if (VoidS(s)) return NULL;
	void *x = s->top->info;
	TList aux = s->top;
	s->top = s->top->next;
	free(aux);
	return x;
}

void PrintStackInfo(TStack s, void f(void *x, FILE *out), FILE *out)
{
	TStack aux = InitS(s->dim);
	while(!VoidS(s))
	{
		void *info = Pop(s);
		f(info, out);
		Push(aux, info);
	}
	while(!VoidS(aux)) Push(s, Pop(aux));
	EmptyS(aux);
}

void EmptyS(TStack s)
{
	while(VoidS(s))
		free(Pop(s));
}
