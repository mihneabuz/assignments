from sys import argv
from re import findall
from copy import copy

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

def updateClause(clause, variable, value):
    if (clause[variable] == value):
        return 1, []
    else:
        newclause = copy(clause)
        newclause[variable] = 0
        if (1 not in newclause) and (-1 not in newclause):
            return 0, []
        return 1, newclause

def updateRepresentation(representation, variable, value):
    newrep = []
    for clause in representation:
        ok, newclause = updateClause(clause, variable, value)
        if (not ok):
            return 0, []
        elif (len(newclause)):
            newrep.append(newclause)
    return 1, newrep

def auxSAT(found, representation, variables):
    if (len(representation) == 0):
        return found
    var = variables.pop()
    x = (auxSAT(*updateRepresentation(representation, var,  1), variables) or
        auxSAT(*updateRepresentation(representation, var, -1), variables))
    variables.append(var)
    return x

def checkSAT(representation, variables):
    return auxSAT(0, representation, [i for i in range(len(variables))])

variables, representation = parseFormula(argv[1])
print(checkSAT(representation, variables))
print(len(variables), len(representation))
