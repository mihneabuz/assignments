module Etapa2 where

import System.Environment (getArgs)

import State
import Regex
import NFA
import DFA

main :: IO ()
main = do 
    [inFile, outFile] <- getArgs
    content <- readFile inFile
    let regex = parsePrenex content
    let nfa = regexToNFA regex
    let dfa = toDFA nfa

    let output = unlines . tail . lines . show $ dfa
    writeFile outFile output

hs = main
