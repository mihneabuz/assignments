    Implementare

Fiind o problema cu complexitate O(N^2), poate fi vazut ca o matrice de N x N,
astfel incat, pentru fiecare i in 0..N si j 0..N se compara distanta dintre
orasul i si orasul j. Fiind o problema simetrica, daca orasul i este in
poximitatea orasului j, automat si orasul j este in proximitatea orasului i.

Un pseudocod optim ar arata asa:
    for i in 0..N:
        for j in i+1..N:
            if dist(oras[i], oras[j]) < range:
                update acc[i]
                update acc[j]

Pentru a putea implementa pe GPU, folosesc o fereastra de 512x512 pe care o
glisez peste matricea N x N, astfel incat sa evit zonele in care nu am nici
o valuare necesara (unde i > j).

Folosec AtomicAdd pentru a acumula in mod atomic valorile.

Dupa ce toate datele au fost calculate, resultatele sunt copiate in memoria
hostului si sunt scrise in fisier.


    Performanta

Testat local pe un GTX 960:
Testul M2 se realizeaza in 4s
Testul H1 se realizeaza in 90s - 100s

Din rularile pe cluster, trec toate testele pe Tesla P100 si Tesla A100,
dar primesc timeout atunci cand rulez pe K40M