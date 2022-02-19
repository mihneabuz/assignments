module Etapa1 where

import Lexer (fromDFAsSpecFile, runLexerVerbose)

import System.Environment (getArgs)

main :: IO ()
main = do 
    [specFile, wordFile] <- getArgs
    
    lexer <- fromDFAsSpecFile specFile
    input <- readFile wordFile
    
    print $ runLexerVerbose lexer input

hs = main
