//Buzatu-Pahontu Mihnea-Nicolae
//315CB

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "util/task_helper.h"

#define MAX_CMD_LENGHT 100
#define MAX_VERSE_COUNT 4
#define MAX_POEM_SIZE 2500
#define RAND_SEED 42

struct Word {
    //structura pe care am folosit-o la task 5, retine un cuvant
    //si pozitia la care incepe in string
    char *word;
    int poz;
};

void cpy(char *s, char *c)
{
    //scrie stringul c peste stringul s
    //strcpy si memcpy imi faceau fite
    int len, i;
    len = strlen(c);
    for (i = 0; i < len; i++)
    {
        s[i] = c[i];
    }
    s[len] = '\0';
    return;
}

int rhy(char *word1, char *word2)
{
    //verifica daca doua cuvinte rimeaza
    if ( word1[ strlen(word1)-1 ] == word2[ strlen(word2)-1 ] ||
         word1[ strlen(word1)-1 ] + 32 == word2[ strlen(word2)-1 ] ||
         word1[ strlen(word1)-1 ] == word2[ strlen(word2)-1 ] + 32 )
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int insert(char *s, char *word, int k)
{
    //insereaza string-ul word in locul primelor k caractere din stringul s
    //si returnneaza numarul de caractere cu care s-a modificat lungimea
    //stringului s
    int len, i;
    char *copie;
    len = strlen(s);
    copie = malloc( len * sizeof(char) );

    len = strlen(word);
    strcpy(copie, s + k);
    strcpy(s, word);
    strcpy(s + len, copie);

    free(copie);
    return ( len - k );
}

int letter(char c)
{
    //verifica daca un caracter este litera sau \n sau \0
    if ( ( c >= 'A' && c <= 'z' ) || c == '\n' || c == '\0' )
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void uppercase(char *s)
{
    //functia de uppercase
    int i, len, ok;
    len = strlen(s);
    i = 1;

    if ( s[0] > 'Z' || s[0] < 'A' )
    {
        s[0] = s[0] - 32;
    }

    while( i < len )
    {
        if ( s[i] == '\n' )
        {
            if ( s[i+1] > 'Z' )
            {
                s[i+1] = s[i+1] - 32;
            }
            if ( s[i+1] != '\n')
            {
                i++;
            }
        }
        else if ( s[i] >= 'A' && s[i] <= 'Z')
        {
            s[i] = s[i] + 32;
        }
        i++;
    }
    return;
}

void trimming(char *s)
{
    //functia de trimming
    int i, j, len;
    i = 0;
    len = strlen(s);
    while( i < len )
    {
        if ( !letter(s[i]) )
        {
            if ( s[i+1] == '\n' || s[i+1] == '\0')
            {
                cpy(s + i, s + i + 1);
            }
            else
            {
                j = i + 1;
                while( !letter(s[j]) )
                {
                    j++;
                }
                if ( s[j] == '\n' || s[j] == '\0')
                {
                    cpy(s + i, s + j);
                }
                else if ( j - i > 0 )
                {
                    cpy(s + i, s + j - 1);
                }
            }
        }
        i++;
    }
    return;
}

void silly(char *s, float prob)
{
    //functia de make_it_silly
    int i, len;
    float sample;
    len = strlen(s);
    for (i = 0; i < len; i++)
    {
        if ( s[i] >= 'A' && s[i] <= 'z' )
        {
            sample = rand() % 100 / 99.0;
            if ( sample < prob )
            {
                if ( s[i] >= 'a' )
                {
                    s[i] = s[i] - 32;
                }
                else
                {
                    s[i] = s[i] + 32;
                }
            }
        }
    }
}

void friendly(char *s)
{
    //functia de make_it_friendly
    //diff este numarul de caractere cu care se modifica stringul s
    //in urma inlocuirii unui cuvant cu diminutivul sau
    int i, j, len, diff;
    char *word, *friendly_word;
    len = strlen(s);
    i = 0;
    while( i < len )
    {
        if ( letter(s[i]) && s[i] != '\n' )
        {
            j = i + 1;
            diff = 0;
            while ( letter(s[j]) && ( s[j] != '\n' && s[j] != '\0' ) )
            {
                j++;
            }

            word = malloc( (j - i) * sizeof(char) );
            word[0] = '\0';
            strncat(word, s + i, j - i);
            get_friendly_word(word, &friendly_word);

            if ( friendly_word != NULL)
            {
                diff = insert(s + i, friendly_word, j - i);
            }
            len = len + diff;
            i = j + diff;
            free(word);
        }
        else
        {
            i++;
        }
    }
    return;
}

int get_rhyming_syn(char *word1, char *word2, char *r_word)
{
    //primeste 2 cuvinte si scrie in r_word cuvantul cel mai scurt
    //lexicografic care este sinonim cu word1, si rimeaza cu word2
    //returneaza 1 daca gaseste un astfel de sinonim si 0 daca nu
    int i, nr_syn;
    char **syn;
    r_word[0] = '\0';
    get_synonym(word1, &nr_syn, &syn);
    for (i = 0; i < nr_syn; i++)
    {
        if ( rhy(word2, syn[i]) )
        {
            if ( r_word[0] == '\0' )
            {
                strcpy(r_word, syn[i]);
            }
            else if ( strcmp(r_word, syn[i]) > 0 )
            {
                strcpy(r_word, syn[i]);
            }
        }
    }
}

void rhyme(char *s, int *rima)
{
    //functia de make_it_rhyme
    //i, j, n sunt contoare
    //n retine numarul de versuri citite (de la 4 la 0)
    //cand ajunge la 0 s-a citit o strofa
    //diff este numarul de caractere cu care se modifica stringul s
    //in urma inlocuirii unui cuvant cu sinonimul sau
    int i, j, n, diff;
    char rhyming_word[12];
    struct Word w[MAX_VERSE_COUNT];
    n = MAX_VERSE_COUNT;
    i = strlen(s);
    while( i >= 0 )
    {
        //aici este modificata o strofa astfel incat sa rimeze
        if ( n == 0 )
        {
            for (j = 1; j <= MAX_VERSE_COUNT; j++)
            {
                diff = 0;
                if ( !rhy(w[j].word, w[rima[j]].word) )
                {
                    get_rhyming_syn(w[j].word, w[rima[j]].word, rhyming_word);
                    if ( rhyming_word[0] != '\0' )
                    {
                        diff = insert(s + w[j].poz, rhyming_word, strlen(w[j].word));
                        for (n = j + 1; n <= MAX_VERSE_COUNT; n++)
                        {
                            w[n].poz = w[n].poz + diff;
                        }
                    }
                }
            }
            n = 4;
        }

        //parcurgerea poeziei si gasirea ultimului cuvant din fiecare vers
        if ( s[i] == '\n' || s[i] == '\0' )
        {
            i--;
            while( !letter(s[i]) || s[i] == '\n' )
            {
                i--;
            }
            j = i - 1;
            while( letter(s[j]) )
            {
                j--;
            }

            w[n].poz = j + 1;
            w[n].word = malloc( (i - j) * sizeof(char) );
            strncpy(w[n].word, s + j + 1, i - j);
            w[n].word[i - j] = '\0';
            n--;

            i = j + 1;
        }
        else
        {
            i--;
        }
    }
    return;
}

int main(void)
{
    //buffer este un pointer in care citest initial poezia
    //cmd este comanda primita in consola interactiva
    //poem retine poezia
    //rima este folosit pentru a da ca paramentru tipul de rima
    //la functia de la task 5
    //run este semaforul pentru consola interactiva
    srand(RAND_SEED);
    float prob;
    int run, *rima;
    char *poem, *buffer, *cmd;
    cmd = malloc(MAX_CMD_LENGHT * sizeof(char));
    run = 1;
    while ( run )
    {
        fgets(cmd, MAX_CMD_LENGHT, stdin);
        if (strstr(cmd, "load"))
        {
            cmd[strlen(cmd) - 1] = '\0';
	    buffer = malloc(MAX_POEM_SIZE * sizeof(char)); 
            load_file(cmd + 5, buffer);

	    poem = malloc( (strlen(buffer) * 1.1) * sizeof(char));
	    strcpy(poem, buffer);
	    free(buffer);
        }
        else if (strstr(cmd, "uppercase"))
        {
            uppercase(poem);
        }
        else if (strstr(cmd, "trimming"))
        {
            trimming(poem);
        }
        else if (strstr(cmd, "make_it_silly"))
        {
            prob = atof(cmd + 14);
            silly(poem, prob);
        }
        else if (strstr(cmd, "make_it_friendlier"))
        {
            friendly(poem);
        }
        else if (strstr(cmd, "make_it_rhyme"))
        {
            rima = malloc((MAX_VERSE_COUNT + 1) * sizeof(int));
            if (strstr(cmd, "imperecheata"))
            {
                rima[1] = 2;
                rima[2] = 1;
                rima[3] = 4;
                rima[4] = 3;
            }
            else if (strstr(cmd, "incrucisata"))
            {
                rima[1] = 3;
                rima[2] = 4;
                rima[3] = 1;
                rima[4] = 2;
            }
            else if (strstr(cmd, "imbratisata"))
            {
                rima[1] = 4;
                rima[2] = 3;
                rima[3] = 2;
		rima[4] = 1;
            }
            rhyme(poem, rima);
	    free(rima);
        }
        else if (strstr(cmd, "print"))
        {
            printf("%s\n\n", poem);
        }
        else if (strstr(cmd, "quit"))
        {
            run = 0;
	    free(poem);
        }
    }
    return 0;
}
