from sys import argv
from re import findall
from itertools import product

def parseFormula(path):
    formula = open(path, 'r').readline().strip()
    variables = list(int(x) for x in set(findall("\d+", formula)))
    variables.sort()

    CNF = []
    for clause in formula.split("^"):
        representation = [0] * len(variables)
        for var in findall("\~{0,1}\d+", clause):
            if (var[0] == '~'):
                representation[variables.index(int(var[1:]))] = -1
            else:
                representation[variables.index(int(var))] = 1
        CNF.append(representation)
    return variables, CNF

def checkInterp(representation, interp):
    for clause in representation:
        if (not checkClause(clause, interp)):
            return 0
    return 1

def checkClause(clause, interp):
    for i in range(len(interp)):
        if (clause[i] == interp[i]):
            return 1
    return 0

def checkSAT(representation, variables):
    for interp in product([1, -1], repeat=len(variables)):
        if (checkInterp(representation, interp)):
            return 1
    return 0

variables, representation = parseFormula(argv[1])
print(checkSAT(representation, variables))
print(len(variables), len(representation))
