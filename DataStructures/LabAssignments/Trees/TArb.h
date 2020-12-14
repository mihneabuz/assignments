#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#ifndef ARBORE_BINAR
#define ARBORE_BINAR

typedef int (*TF)(int);

typedef struct node{
    int info;
    struct node *st;
    struct node *dr;
}TNod, *TArb;

TArb create_random_tree(int num);
TArb create_ordered_tree(int size);
TArb create_node(int x, TArb l, TArb r);
void distruge(TArb r);
void distrugeArb(TArb *a);

#ifndef _random_
#define _random_
#define random(n) (rand() % (n))
#define randomize() (srand((unsigned int)time(NULL)))
#endif
#endif
