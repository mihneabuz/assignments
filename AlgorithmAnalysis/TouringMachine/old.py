import sys

class TouringMachine:

    #Buzatu-Pahontu Mihnea-Nicolae 324CB
    #
    #Am folosit o implementare OOP, in care MT retine numarul starilor si
    #starea curenta, o lista cu starile finale, un dictionar 2-diemnsional 
    #cu tranzitiile, dar si banda infinita pe care se afla un cuvant, 
    #pe care o parcurge folosind un indice curent.
    #
    #
    def __init__(self):
        self.no_states = 0
        self.final_states  = []
        self.transitions = 0
        self.word = list("aababb")
        self.current_state = int(0);
        self.current_index = 0;

    #fucntie de citire a unui TM dintr-un fisier, folosita pentru debugging
    def read(self, fileName):
        data = open(fileName, 'r')
        n = int(data.readline())
        buffer = data.readline()
        if (buffer == '-\n'):
            final = ()
        else:
            final = tuple([int(x) for x in data.readline().split()])

        #initializez dictionarul 2d
        self.transitions = {state: {chr(letter): 0
                                    for letter in [*(range(65, 91)), 35]}
                            for state in range(0, n)}
        buffer = data.readline()
        while(buffer):
            trans = buffer.split()
            trans[0] = int(trans[0])
            trans[2] = int(trans[2])
            self.transitions[trans[0]][trans[1]] = tuple(trans[2 :])
            buffer = data.readline()

        data.close()
        self.no_states = n
        self.final_states = final
        return 1

    #functie de citire a unui TM din stdin, folosita pentru teste
    def read(self):
        n = int(sys.stdin.readline())
        buffer = sys.stdin.readline()
        if (buffer == '-\n'):
            final = ()
        else:
            final = tuple([int(x) for x in buffer.split()])

        #initializez dictionarul 2d
        self.transitions = {state: {chr(letter): 0
                                       for letter in [*(range(65, 91)), 35]}
                            for state in range(0, n)}
        buffer = sys.stdin.readline()
        while(buffer):
            trans = buffer.split()
            trans[0] = int(trans[0])
            trans[2] = int(trans[2])
            self.transitions[trans[0]][trans[1]] = tuple(trans[2 :])
            buffer = sys.stdin.readline()

        self.no_states = n
        self.final_states = final
        return 1

    #printeaza toate informatiile TM-ului, pentru debugging
    def info(self):
        print("Numar stari: ", self.no_states)
        print("Stari finale: ", self.final_states)
        print("Tranzitii: ")
        for i in range(0, self.no_states):
            for j in self.transitions[i]:
                if (self.transitions[i][j] != 0):
                    print("({}, '{}') -> {}"
                          .format(i, j, self.transitions[i][j]))

    #printeaza configuratia curenta a TM-ului, pentru debugging
    def current_config(self):
        if (not self.word):
            print("Empty word")
        else:
            print("{} {} {}".format(self.word[0 : self.current_index],
                                    self.current_state,
                                    self.word[self.current_index:]))

    #functie statica care printeaza o configuratie in format cerut de checker
    def print_config(state):
        if (state == False):
            print(state, end = ' ')
            return False
        s = ""
        print("({},{},{})".format(
                s.join(state[0]), state[1], s.join(state[2])),
              end = ' ')


    #simuleaza un pas pe configuratia primita ca argument
    def simulate_step(self, config):
        trans = self.transitions[config[1]][
                                config[2][0]]

        if (trans == 0):
            return False
        q = trans[0]
        config[2][0] = trans[1]
        u = list(config[0])
        v = list(config[2])
        if (trans[2] == 'L'):
            v.insert(0, u.pop())
            if (not u):
                u = '#'
        elif (trans[2] == 'R'):
            u.append(v.pop(0))
            if (not v):
                v = '#'
        return (u, q, v)

    #simuleaza un pas pe configuratia interna a TM-ului
    def step(self):
        trans = self.transitions[self.current_state][
                                self.word[self.current_index]]
        if (trans == 0):
            return False
        self.current_state = trans[0]
        self.word[self.current_index] = trans[1]
        if (trans[2] == 'L'):
            if (self.current_index == 0):
                self.word.insert(0, '#')
            else:
                self.current_index -= 1
        elif(trans[2] == 'R'):
            if (self.current_index == len(self.word) - 1):
                self.word.append('#');
            self.current_index += 1
        return True

    #Verifica acceptarea unui cuvant de catre TM
    def accept(self, word):
        self.word = list(word);
        self.current_index = 0;
        self.current_state = 0;
        while(self.step()): #daca ajunge in stare finala, accepta
            if (self.current_state in self.final_states):
                return True

        return False

    #Verifica acceptarea unui cuvant de catre TM in k pasi
    def k_accept(self, k, word):
        self.word = list(word);
        self.current_index = 0;
        self.current_state = 0;
        for i in range(0, k):
            self.step() #daca ajunge in stare finala, accepta
            if (self.current_state in self.final_states):
                return True

        return False

#modifica formatul unei configuratii citite pentru a putea fi
#pasata functiei de simulate_step
def format_input(string):
    inp = string.split(',')
    inp[0] = list(inp[0])
    inp[1] = int(inp[1])
    inp[2] = list(inp[2])
    return inp

#main-ul
#initializez un TM, verific ce fel de test este si citesc/afisez 
#informatiile necesare
tm = TouringMachine()
test = input()
if (test == "step"):
    buffer = input()
    configs = buffer.split(") (")
    configs[0] = configs[0][1:]
    configs[len(configs) - 1] = configs[len(configs) - 1][0:-1]
    tm.read()
    for i in configs:
        TouringMachine.print_config(tm.simulate_step(format_input(i)))

elif (test == 'accept'):
    buffer = input()
    words = buffer.split()
    tm.read()
    for i in words:
        print(tm.accept(i), end = ' ')

elif (test == 'k_accept'):
    buffer = input()
    words_ks = [ele.split(',') for ele in buffer.split()]
    tm.read()
    for i in words_ks:
        print(tm.k_accept(int(i[1]), i[0]), end = ' ')
