module Regex (parsePrenex, parsePostnex,
              parseRegex, regexToNFA
             ) where

--------------------------IMPORTS--------------------------
import NFA
import State
import Converter

import Data.Char (chr, ord)
-----------------------------------------------------------


-----------------------DECLARATION------------------------ 
data Regex = Simple Char
           | Star Regex
           | Plus Regex
           | Concat Regex Regex
           | Union  Regex Regex
           | Range  Char  Char

instance Show Regex where
  show (Star r)       = show r ++ "*"
  show (Plus r)       = show r ++ "+"
  show (Concat r1 r2) = "(" ++ show r1 ++ show r2 ++ ")"
  show (Union  r1 r2) = "(" ++ show r1 ++ "|" ++ show r2 ++ ")"
  show (Range  c1 c2) = "[" ++ [c1] ++ "-" ++ [c2] ++ "]"
  show (Simple c)     = [c]
-----------------------------------------------------------


-----------------------INPUT PARSING----------------------- 
parsePrenex :: String -> Regex
parsePrenex = fst . head . parseRegex'
    where parseRegex' str = do
            (lexem0, rest0) <- lex str
            case lexem0 of 
                "STAR" -> do
                    (regex1, rest1) <- parseRegex' rest0
                    return (Star regex1, rest1)
                "PLUS" -> do
                    (regex1, rest1) <- parseRegex' rest0
                    return (Plus regex1, rest1)
                "CONCAT" -> do
                    (regex1, rest1) <- parseRegex' rest0
                    (regex2, rest2) <- parseRegex' rest1
                    return (Concat regex1 regex2, rest2)
                "UNION" -> do
                    (regex1, rest1) <- parseRegex' rest0
                    (regex2, rest2) <- parseRegex' rest1
                    return (Union regex1 regex2, rest2)
                "RANGE" -> do
                    ([char1], rest1) <- lex rest0
                    ([char2], rest2) <- lex rest1
                    return (Range char1 char2, rest2)
                _ -> return (Simple (head lexem0), rest0)

parsePostnex :: String -> Regex
parsePostnex = fst . head . parseRegex' . unwords . reverse . words
    where parseRegex' str = do
            (lexem0, rest0) <- lex str
            case lexem0 of 
                "STAR" -> do
                    (regex1, rest1) <- parseRegex' rest0
                    return (Star regex1, rest1)
                "PLUS" -> do
                    (regex1, rest1) <- parseRegex' rest0
                    return (Plus regex1, rest1)
                "CONCAT" -> do
                    (regex1, rest1) <- parseRegex' rest0
                    (regex2, rest2) <- parseRegex' rest1
                    return (Concat regex2 regex1, rest2)
                "UNION" -> do
                    (regex1, rest1) <- parseRegex' rest0
                    (regex2, rest2) <- parseRegex' rest1
                    return (Union regex2 regex1, rest2)
                "RANGE" -> do
                    (char1, rest1) <- lex rest0
                    (char2, rest2) <- lex rest1
                    return (Range (read char2) (read char1), rest2)
                _ -> return (Simple (read lexem0), rest0)

parseRegex :: String -> Regex
parseRegex = parsePostnex . infixToPostnex
-----------------------------------------------------------


--------------------------FUNCTIONS------------------------ 
regexToNFA :: Regex -> NFA
regexToNFA regex = fst $ regexToNFA' (regex, 0)
    where
        regexToNFA' :: (Regex, Int) -> (NFA, Int)
        regexToNFA' (Simple c, x) = (NFA s1 s2 [Normal s1 c s2], x+2)
            where (s1, s2) = (SimpleState x, SimpleState (x+1))

        regexToNFA' (Star r, x) = starNFA nfa x1
            where (nfa, x1) = regexToNFA' (r, x)

        regexToNFA' (Plus r, x) = plusNFA nfa x1
            where (nfa, x1) = regexToNFA' (r, x)

        regexToNFA' (Concat r1 r2, x) = concatNFA nfa1 nfa2 x2
            where (nfa1, x1) = regexToNFA' (r1, x) 
                  (nfa2, x2) = regexToNFA' (r2, x1)

        regexToNFA' (Union r1 r2, x) = unionNFA nfa1 nfa2 x2
            where (nfa1, x1) = regexToNFA' (r1, x) 
                  (nfa2, x2) = regexToNFA' (r2, x1)

        regexToNFA' (Range c1 c2, x) = rangeNFA c1 c2 x

concatNFA :: NFA -> NFA -> Int -> (NFA, Int)
concatNFA (NFA start1 final1 trans1) (NFA start2 final2 trans2) x = 
    (NFA start1 final2 (trans1 ++ trans2 ++ [Epsilon final1 start2]), x)

starNFA :: NFA -> Int -> (NFA, Int)
starNFA (NFA start final trans) x = (NFA newStart newFinal (newTrans ++ trans), x + 2)
    where newStart = SimpleState x
          newFinal = SimpleState (x+1)

          newTrans = [ Epsilon newStart start
                     , Epsilon final newFinal
                     , Epsilon newStart newFinal
                     , Epsilon final start ]

plusNFA :: NFA -> Int -> (NFA, Int)
plusNFA (NFA start final trans) x = (NFA newStart newFinal (newTrans ++ trans), x + 2)
    where newStart = SimpleState x
          newFinal = SimpleState (x+1)

          newTrans = [ Epsilon newStart start
                     , Epsilon final newFinal
                     , Epsilon final start ]

unionNFA :: NFA -> NFA -> Int -> (NFA, Int)
unionNFA (NFA s1 f1 t1) (NFA s2 f2 t2) x = (NFA newStart newFinal (newTrans ++ t1 ++ t2), x + 2)
    where newStart = SimpleState x
          newFinal = SimpleState (x+1)

          newTrans = [ Epsilon newStart s1
                     , Epsilon newStart s2
                     , Epsilon f1 newFinal
                     , Epsilon f2 newFinal ]

rangeNFA :: Char -> Char -> Int -> (NFA, Int)
rangeNFA c1 c2 x = (NFA start final trans, x+2)
    where start = SimpleState x
          final = SimpleState (x+1)
          trans = map (\c -> Normal start c final) [c1..c2]
-----------------------------------------------------------
