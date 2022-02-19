#!/bin/bash

stack build --copy-bins

cp build/Etapa2 test/T2/Main

cd test/T2

python3 checker.py haskell
