#include "TArb.h"
#include <math.h>

#define log2(x) (log(((double)x)) / log(2.0))
#define kMax 100

TArb create_random_tree(int num)
/* generează un arbore cu elemente alese random și fiecare fiu al unui nod este ales random */
{
	int niv;
    int lim = ceil(log2(num)) + 1;
	TArb a = NULL, *aa;
	srand(time(NULL));
	while (num > 0) {
		for (niv = 0, aa = &a; niv < lim ; niv++)
			if (!(*aa)) {
				*aa = create_node(rand() % kMax, NULL, NULL);
                num--;
				break;
			} else
		aa = random(2) ? &(*aa)->dr : &(*aa)->st;
	}
	return a;
}

TArb ordered_tree(int num, int curr_level, int lim, int restart)
/* generează un arbore ale cărui elemente vor fi întotdeauna [0 1 2..] la parcurgerea acestuia în preordine */
{
    static int start = 0;
    if(restart)
        start = 0;
    if(start >= num || curr_level > lim)
        return NULL;
    int val = start++;
    if(curr_level < lim){
        TArb l = ordered_tree(num, curr_level + 1, lim, 0);
        TArb r = ordered_tree(num, curr_level + 1, lim, 0);
        TArb a = create_node(val, l, r);
        return a;
    }
    return create_node(val, NULL, NULL);
}

TArb create_ordered_tree(int size){
    return ordered_tree(size, 0, floor(log2((double)size)), 1);
}

TArb create_node(int x, TArb l, TArb r){
	TArb aux = (TArb)malloc (sizeof(TNod));
	if (!aux) return NULL;
	  aux->info = x;
    aux->st = l;
    aux->dr = r;

	return aux;
}

void distruge(TArb a)
{
    if(a == NULL)
        return;
    distruge(a->st);
    distruge(a->dr);
    free(a);
}

void distrugeArb(TArb *a)
{
	distruge(*a);
	*a = NULL;
}
