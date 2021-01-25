
#
# Simple implementation. exponential clauses
#


from itertools import combinations
from sys import stdin

K = int(stdin.readline().strip())
N = int(stdin.readline().strip())
M = int(stdin.readline().strip())

V = tuple([i + 1 for i in range(N)])

strings = []
for clause in combinations(V, N - K + 1):
    strings.append("(" + "V".join(str(x) for x in clause) + ")")

for clause in combinations(V, K + 1):
    strings.append("(" + "V".join("~" + str(x) for x in clause) + ")")

for clause in stdin:
    strings.append("(" + clause.strip().replace(" ", "V") + ")")

print("^".join(strings))
