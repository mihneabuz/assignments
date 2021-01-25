import os
import sys
import subprocess
from time import time

assert (len(sys.argv) >= 2), "Calling format should be: \"python3 checker.py [simple/bdd] [optional: input file path]\""

print("-- Tema 3 AA checker --\n")

if sys.argv[1] == 'simple':
    prog_name = "simple_sat.py"
    print("Testing the simple SAT solver:")
elif sys.argv[1] == 'bdd':
    prog_name = "bdd_sat.py"
    print("Testing the BDD-SAT solver:")
else:
    raise Exception("Calling format should be: \"python3 checker.py [simple/bdd] [optional: input file path]\"")

if len(sys.argv) == 3:
    input_files = [sys.argv[2]]
else:
    input_files = os.listdir("testcases/input/")
    input_files.sort()
    input_files = list(map(lambda s : "testcases/input/" + s, input_files))
total = 0

for input_file_path in input_files:
    begin = time()
    satprocess = subprocess.Popen(['python3', prog_name, input_file_path], stdout=subprocess.PIPE)
    result = satprocess.communicate(timeout=600)
    exectime = time() - begin

    out = result[0].decode("utf-8").strip()
    out, size = out.split("\n")

    if not result[1] is None:
        err = result[1].decode("utf-8")
    else:
        err = None

    ref_file_path = "testcases/ref/ref" + input_file_path[-6:]
    with open(ref_file_path, 'r') as fd:
        ref = fd.readline().strip()
        if out == ref:
            total += 10
            print("[Test " + input_file_path[-6:-4] + "]: Passed [10p] Time: [{:.3f}] Size: [".format(exectime) + size + "]")
        else:
            print("[Test " + input_file_path[-6:-4] + "]: Failed [0p]\n" + str(err))

if len(sys.argv) == 2:
    print("Total:", total)
