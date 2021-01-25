#
#Implementation using a Sequantial Counter
#variables -> O(k*n)
#clauses -> O(k*n)
#
#Carsten Sinz "Towards an Optimal CNF Encoding ofBoolean Cardinality Constraints"
#

from itertools import combinations
from sys import stdin

K = int(stdin.readline().strip())
N = int(stdin.readline().strip())
M = int(stdin.readline().strip())

V = tuple([i + 1 for i in range(N)])

strings = []
registers = [[N + 1 + j + i * K for j in range(K)] for i in range(N-1)]

strings.append("(~" + str(1) + "V" + str(registers[0][0]) + ")")
for j in range(1, K):
    strings.append("(~" + str(registers[0][j]) + ")")

for i in range(1, N - 1):
    strings.append("(~" + str(i + 1) + "V" + str(registers[i][0]) + ")")
    strings.append("(~" + str(registers[i-1][0]) + "V" +
                    str(registers[i][0]) + ")")
    for j in range(1, K):
        strings.append("(~" + str(i + 1) + "V~" + str(registers[i-1][j-1]) +
                       "V" + str(registers[i][j]) + ")")
        strings.append("(~" + str(registers[i-1][j]) + "V" +
                       str(registers[i][j]) + ")")
    strings.append("(~" + str(i + 1) + "V~" + str(registers[i-1][K-1]) + ")")
strings.append("(~" + str(N) + "V~" + str(registers[N-2][K-1]) + ")")

for clause in stdin:
    strings.append("(" + clause.strip().replace(" ", "V") + ")")

print("^".join(strings))
