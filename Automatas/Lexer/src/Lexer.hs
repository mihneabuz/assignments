module Lexer (
    Lexer,
    fromDFAsSpec,     fromDFAsSpecFile,
    fromRegexSpec,    fromRegexSpecFile,
    runLexer,         runLexerOnFile,
    runLexerVerbose,  runLexerVerboseOnFile
             ) where

--------------------------IMPORTS--------------------------
import DFA
import NFA
import Regex
import State

import Data.List.Split
-----------------------------------------------------------


-----------------------DECLARATION------------------------ 
type Lexer = (String -> [(Token, Lexem)])
data MatchResult = Matched Int | EOF | ErrorAfter Int deriving (Show, Eq)
type Token = String
type Lexem = String

instance Ord MatchResult where
   compare (Matched count1) (Matched count2) = compare count1 count2
   compare (Matched count) _ = GT
   compare _ (Matched count) = LT
   compare EOF _ = GT
   compare _ EOF = LT
   compare (ErrorAfter count1) (ErrorAfter count2) = compare count1 count2
-----------------------------------------------------------


--------------------------INTERFACE------------------------ 
fromDFAsSpec :: String -> Lexer
fromDFAsSpec spec = run $ fromDFAs spec

fromRegexSpec :: String -> Lexer
fromRegexSpec spec = run $ fromRegex spec

fromDFAsSpecFile :: FilePath -> IO Lexer
fromDFAsSpecFile specFile = do
    spec <- readFile specFile
    return (fromDFAsSpec spec)

fromRegexSpecFile :: FilePath -> IO Lexer
fromRegexSpecFile specFile = do
    spec <- readFile specFile
    return (fromRegexSpec spec)

runLexer :: Lexer -> String -> [(Token, Lexem)]
runLexer lexer = lexer

runLexerVerbose :: Lexer -> String -> String
runLexerVerbose lexer input = showTokensVerbose $ lexer input

runLexerOnFile :: Lexer -> FilePath -> IO [(Token, Lexem)]
runLexerOnFile lexer inputFile = do
    input <- readFile inputFile
    return (lexer input)

runLexerVerboseOnFile :: Lexer -> FilePath -> IO String
runLexerVerboseOnFile lexer inputFile = do
    input <- readFile inputFile
    return (showTokensVerbose $ lexer input)
----------------------------------------------------------- 


---------------------------PARSING------------------------- 
fromDFAs :: [Char] -> [DFA] 
fromDFAs = map parseDFA . splitOn "\n\n"

fromRegex :: String -> [DFA]
fromRegex = map (dfa . takeWhile (/= ';')) . safeLines
    where 
        dfa :: String -> DFA
        dfa str = toDFAWithName token . regexToNFA . parseRegex $ regex
            where token = head . words $ str
                  regex = drop (length token) str
-----------------------------------------------------------


--------------------------FUNCTIONS------------------------ 
match :: DFA -> [Char] -> MatchResult
match dfa@(DFA _ _ initialState _ _ _) str = matchHelper dfa (Just (initialState, str)) 0
  where
    matchHelper _ Nothing count = ErrorAfter (count - 1)
    matchHelper dfa (Just (state, [])) count = if inFinalState dfa (state, []) then Matched count else EOF
    matchHelper dfa (Just config) count = if currentMatch >= nextMatch then currentMatch else nextMatch
      where
        currentMatch = if inFinalState dfa config && count > 0 then Matched count else ErrorAfter count
        nextMatch = matchHelper dfa (safeStep dfa config) (count + 1)

run :: [DFA] -> [Char] -> [(Token, Lexem)]
run dfaList [] = []
run dfaList word = case snd best of
  Matched count    -> (fst best, take count word) : run dfaList (drop count word)
  ErrorAfter count -> [("Error", take count word)]
  EOF              -> [("Error", "EOF")]
  where
    matches = map (\dfa -> (getToken dfa, match dfa word)) dfaList
    best = foldl1 (\m1 m2 -> if snd m1 >= snd m2 then m1 else m2) matches
----------------------------------------------------------- 


---------------------------HELPERS------------------------- 
safeLines :: String -> [String]
safeLines = _safeLines [[]]
    where
        _safeLines :: [String] -> String -> [String]
        _safeLines acc [] = reverse $ map reverse acc
        _safeLines acc ['\n'] = _safeLines acc []
        _safeLines (line:acc) ('\\':'n':rest) = _safeLines (('\n':line):acc) rest 
        _safeLines acc ('\n':rest) = _safeLines ([]:acc) rest 
        _safeLines (line:acc) (c:rest) = _safeLines ((c:line):acc) rest 
        _safeLines _ _ = error "safeLines bad input"

getToken :: DFA -> Token
getToken (DFA token _ _ _ _ _) = token

showTokensVerbose :: [(Token, Lexem)] -> String
showTokensVerbose tokens = do
  case containsError tokens of
    Just error -> showError error
    Nothing -> processTokens tokens

processTokens :: [(Token, Lexem)] -> String
processTokens = init . foldr (\(token, lexem) acc -> token ++ " " ++ escapeChars lexem ++ "\n" ++ acc) ""
  where
    escapeChars :: String -> String
    escapeChars = foldr (\c acc -> replace c ++ acc) ""
      where 
        replace '\n' = "\\n"
        replace   c  =  [c]

containsError :: [(Token, Lexem)] -> Maybe (Int, Int, String)
containsError tokens = findError tokens (0, 0)
  where
    findError :: [(Token, Lexem)] -> (Int, Int) -> Maybe (Int, Int, String)
    findError [] _ = Nothing
    findError ((token, lexem):xs) (lineNr, colNr)
      | token == "Error" = Just (lineNr, colNr, lexem)
      | otherwise = findError xs (foldl helper (lineNr, colNr) lexem)

    helper (lineNr, colNr) c
      | c == '\n' = (lineNr + 1, 0)
      | otherwise = (lineNr, colNr + 1)

showError :: (Int, Int, String) -> String
showError (lineNr, colNr, text)
  | text == "EOF" = "No viable alternative at character EOF, line " ++ show lineNr
  | otherwise = "No viable alternative at character " ++ show (colNr + length text) ++ ", line " ++ show lineNr
----------------------------------------------------------- 
