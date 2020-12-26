//Buzatu-Pahontu Mihnea-Nicolae
//315CB

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.c"
#include "structures.c"

#define MAX_CMD_LEN 16
#define MAX_URL_LEN 22
#define DEFAULT_BANDWITH 1024
#define MAX_HISTORY 256


void PrintOpenTabs(char *x, int ind, FILE *out) {
	//formatul de printare pentru print_open_tabs
	fprintf(out, "(%d: %s)\n", ind, x);
}

void PrintHistoryURL(void *x, FILE *out) {
	//formatul de printare pentru history
	fprintf(out, "%s\n", (char*)x);
}

void PrintPageResources(TPage p, FILE *out)
{
	//printeaza toate resursele paginii p
	int i;
	for (i = 0; i < p->res_num; i++)
		fprintf(out, "[%d - \"%s\" : %ld]\n", i,
				p->resources[i].name,
				p->resources[i].dimension);
}

void PrintDownloads(void *res, FILE *out)
{
	//formatul de printare pentru downloads
	if (((Resource*)res)->dimension == ((Resource*)res)->currently_downloaded)
		fprintf(out, "[\"%s\" : completed]\n", ((Resource*)res)->name);
	else fprintf(out, "[\"%s\" : %ld/%ld]\n",
			((Resource*)res)->name,
			((Resource*)res)->dimension-((Resource*)res)->currently_downloaded,
			((Resource*)res)->dimension);
}

int CompareResources(void *a, void *b) {
	//compara doua elemente de timp Resource, folosita pt lista de prioritati
	return ((Resource*)a)->dimension - ((Resource*)a)->currently_downloaded
			- ((Resource*)b)->dimension + ((Resource*)b)->currently_downloaded;
}

void DelHistory(TQueue q, int n)
{
	//sterge primele n elemente din coada q
	int i;
	for(i = 0; i < n && (!VoidQ(q)); i++)
		free(Dequeue(q));
}

int GoBack(TabStruct tab)
{
	//pune pagina curenta in forward si prima pagina din back in tabul curent
	if (VoidS((tab->back))) return 0;
	else
	{
		Push(tab->forward, tab->page);
		tab->page = Pop(tab->back);
	}
	return 1;
}

int GoForward(TabStruct tab)
{
	//pune pagina curenta in back si prima pagina din forward in tabul curent
	if (VoidS(tab->forward)) return 0;
	else
	{
		Push(tab->back, tab->page);
		tab->page = Pop(tab->forward);
	}
	return 1;
}

void EmptyPageStack(TStack s)
{
	//goleste si elibereaza o stiva de pagini
	TPage aux;
	while(!VoidS(s))
	{
		aux = Pop(s);
		FreePage(aux);
	}
}

TabStruct DeleteLastTab(TabStruct tabs, TabStruct current)
{
	//sterge ultimul tab deschis
	//daca acesta este chiar tabul curent, returneaza tabul de dinaintea
	//tabului curent
	if (current->next == NULL)
	{
		if (tabs == current)
		{
			FreeTab(current);
			tabs = current = NULL;
		}
		else
		{
			RemoveEnd(&tabs);
			for (current = tabs;
				current->next != NULL;
				current = current->next);
			current->next = NULL;
		}
	}
	else RemoveEnd(&tabs);
	return current;
}

void ProcessDownloads(TQueue current, TQueue finished, unsigned int down_amount)
{
	//proceseaza download-urile, mutand toate descarcarile terminate in coada
	//finished
	//down_amount este marimea descarcata (bandwith * time)
	//current este coada de descarcari curente si finished este coada de 
	//descarcari termiante
	if (VoidQ(current)) return;
	Resource *aux = Peek(current);
	while(down_amount >= (aux->dimension - aux->currently_downloaded))
	{
		down_amount = down_amount - aux->dimension + aux->currently_downloaded;
		aux->currently_downloaded = aux->dimension;
		Enqueue(finished, Dequeue(current));
		aux = Peek(current);
	}
	aux->currently_downloaded = aux->currently_downloaded + down_amount;
}

int main(int argc, char *argv[])
{
	//deschiderea fisierelor
	if (argc < 3) 
	{
		fprintf(stderr, "files not found\n");
		return -1;
	}
	FILE *in = fopen(argv[1], "rt");
	FILE *out = fopen(argv[2], "wt");
	if (!in || !out) 
	{
		fprintf(stderr, "files not found\n");
		return -1;
	}
	
	//declararea si initializarea variabilelor
	char *CMD, *NEW_URL;
	CMD = malloc(MAX_CMD_LEN * sizeof(char));
	TabStruct TABS = NULL,
 			  CURRENT_TAB = AddNewTab(&TABS);
	TQueue HISTORY = InitQ(sizeof(char*)),
		   DOWNLOADS = InitQ(sizeof(Resource)),
		   COMPLETED_DOWNLOADS = InitQ(sizeof(Resource));
	unsigned int BANDWITH = DEFAULT_BANDWITH;
	int INT_ARG;
	Resource* COPY;

	while(fscanf(in, "%s", CMD) != EOF)
	{
		//loop-ul principal al programului
		if (!strcmp(CMD, "goto")) {
			NEW_URL = malloc(MAX_URL_LEN * sizeof(char));
			fscanf(in, "%s", NEW_URL);

			if (CURRENT_TAB->page != NULL)
				Push(CURRENT_TAB->back, CURRENT_TAB->page);

			CURRENT_TAB->page = NewPage(NEW_URL);
			EmptyPageStack(CURRENT_TAB->forward);
			Enqueue(HISTORY, NEW_URL);
			ProcessDownloads(DOWNLOADS, COMPLETED_DOWNLOADS, BANDWITH);
		}
		else if (!strcmp(CMD, "newtab")) CURRENT_TAB = AddNewTab(&TABS);
		else if (!strcmp(CMD, "history"))
			PrintQueueInfo(HISTORY, PrintHistoryURL, out);
		else if (!strcmp(CMD, "print_open_tabs"))
			PrintTabInfo(TABS, PrintOpenTabs, out);
		else if (!strcmp(CMD, "change_tab")) {
			fscanf(in, "%d", &INT_ARG);
			CURRENT_TAB = GetTab(TABS, INT_ARG);
		}
		else if (!strcmp(CMD, "deltab"))
			CURRENT_TAB = DeleteLastTab(TABS, CURRENT_TAB);
		else if (!strcmp(CMD, "back")) GoBack(CURRENT_TAB);
		else if (!strcmp(CMD, "forward")) GoForward(CURRENT_TAB);
		else if (!strcmp(CMD, "list_dl")) {
			if (CURRENT_TAB->page) PrintPageResources(CURRENT_TAB->page, out);
		}
		else if (!strcmp(CMD, "del_history")) {
			fscanf(in, "%d", &INT_ARG);
			if (INT_ARG == 0) DelHistory(HISTORY, MAX_HISTORY);
			DelHistory(HISTORY, INT_ARG);
		}
		else if (!strcmp(CMD, "set_band")) fscanf(in, "%u", &BANDWITH);
		else if (!strcmp(CMD, "wait")) {
			fscanf(in, "%d", &INT_ARG);
			ProcessDownloads(DOWNLOADS, COMPLETED_DOWNLOADS, INT_ARG*BANDWITH);
		}
		else if (!strcmp(CMD, "download")) {
			//adauga o copie a resursei la coada de descarcari
			fscanf(in, "%d", &INT_ARG);
			if (CURRENT_TAB->page) {
				COPY = CopyResource(CURRENT_TAB->page->resources[INT_ARG]);
				InsertPriorityQ(DOWNLOADS, COPY, CompareResources);
			}
		}
		else if (!strcmp(CMD, "downloads"))	{
			PrintQueueInfo(DOWNLOADS, PrintDownloads, out);
			PrintQueueInfo(COMPLETED_DOWNLOADS, PrintDownloads, out);
		}
	}
	//Eliberarea memoriei
	FreeAllTabs(TABS);
	FreeQ(HISTORY);
	FreeQ(DOWNLOADS);
	FreeQ(COMPLETED_DOWNLOADS);
	free(CMD);
	//inchiderea fisierelor
	fclose(in);
	fclose(out);
	return 0;
}
