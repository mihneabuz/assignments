Buzatu Pahontu Mihnea Nicolae
334CB
03.11.2021

Tema 1 APD - Algoritmi Genetici

Pornind de la implementarea secventiala, am paralelizat astfel:
    - Fiecare thread primeste:
        * un id
        * numarul total de threaduri
        * indecsi de start si end pentru protiunea care ii apartine
        * numarul de cromozomi, generatii si capacitatea rucsacului
        * un pointer catre array-ul de obiecte 
        * 2 buffere, current si next generation
        * o masca de biti care reprezinta sarcinile threadului
        * un pointer catre o resursa comuna care ajuta la sincronizarea sortarii
        * o bariera pentru sincronizarea generala

    - inainte sa fie create threadurile:
        * calculez pentru fiecare un index de start si de end in generatie
        * atribui unor threaduri sarcini suplimentarea prin compute_flags

    - La fiecare generatie:
        * fiecare thread isi calculeaza fitness-ul si sorteaza elementele din
          portiunea care ii apartine folosind qsort
        * segmentele sortate sunt combinate cu ajutorul resursei comune de merge
          controller care retine indecsi protiunilor sortate
        * sunt calculati indivizii din next generation, fiecare thread contribuie
          in functie de flag-urile pe care le-a primit in compute_flags
        * fiecare thread isi reindexeaza portiunea aferenta

    - sincronizarea se face folosind o bariera si o resursa comuna merge_controller
    - resursa comuna nu este protejata de un mutex deoarece majoritatea operatiilor
      asupra ei sunt de read, asa ca am decis sa o protejez folosind bariera si
      atribuind unui singur thread sarcina de scriere


rezultate rulare checker local:

Se ruleaza implementarea secventiala...
OK

Se ruleaza testele pentru P=2...
OK

Se ruleaza testele pentru P=3...
OK

Se ruleaza testele pentru P=4...
OK

Teste corecte: 12/12
Acceleratie 1-2: 2.23
Acceleratie 1-4: 3.30
Acceleratie 2-4: 1.48

Scor scalabilitate: 50/50
Scor corectitudine: 30/30
Scor total:         80/80
