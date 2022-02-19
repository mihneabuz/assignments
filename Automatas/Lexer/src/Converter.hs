module Converter (infixToPostnex) where

--------------------------IMPORTS--------------------------
import Data.List (intercalate)
-----------------------------------------------------------


-----------------------DECLARATION------------------------ 
data Token = Unary Char
           | Binary Char
           | Operand OP
           | OpenParen
           | CloseParen
            deriving (Show, Eq)

data OP = Simple Char
        | Range Char Char
         deriving (Show, Eq)
-----------------------------------------------------------


--------------------------FUNCTIONS------------------------ 
verboseOperator :: Token -> String
verboseOperator (Binary '|') = "UNION"
verboseOperator (Binary ':') = "CONCAT"
verboseOperator (Unary  '*') = "STAR"
verboseOperator (Unary  '+') = "PLUS"

verboseOperand :: OP -> String
verboseOperand (Simple c) = show c
verboseOperand (Range c1 c2) = show c1 ++ " " ++ show c2 ++ " RANGE"

precedence :: Token -> Int
precedence (Binary '|') = 1
precedence (Binary ':') = 2
precedence OpenParen = 0

infixToPostnex :: String -> String
infixToPostnex = toPostnex [] [] . implicitConcat . tokenize . strip

_dumpStack = unwords . map verboseOperator
_precedenceHigherThan c o = precedence o > precedence c

toPostnex :: String -> [Token] -> [Token] -> String
toPostnex out stack [] = out ++ _dumpStack stack
toPostnex out stack (token:rest) = case token of
        (Unary op)   -> toPostnex (out ++ verboseOperator (Unary op) ++ " ") stack rest
        (Binary op)  -> let (higher, lower) = span (_precedenceHigherThan token) stack
                        in toPostnex (out ++ _dumpStack higher ++ " ") (Binary op:lower) rest

        OpenParen    -> toPostnex out (OpenParen:stack) rest
        CloseParen   -> let (before, paren:after) = break (== OpenParen) stack
                        in toPostnex (out ++ _dumpStack before ++ " ") after rest

        (Operand op) -> toPostnex (out ++ verboseOperand op ++ " ") stack rest
-----------------------------------------------------------


--------------------------HELPERS--------------------------
strip :: String -> String
strip [] = []
strip ('\'':c:'\'':xs) = '\'' : c : '\'' : strip xs
strip (x:xs) = if x == ' ' then strip xs else x : strip xs

tokenize :: String -> [Token]
tokenize [] = []
tokenize (c:cs) = case c of
    '|' -> Binary c   : tokenize cs
    '*' -> Unary c    : tokenize cs
    '+' -> Unary c    : tokenize cs
    '(' -> OpenParen  : tokenize cs
    ')' -> CloseParen : tokenize cs
    '[' -> Operand (Range (cs !! 0) (cs !! 2)) : tokenize (drop 4 cs)
    '\''-> Operand (Simple (head cs)) : tokenize (drop 2 cs)
    _   -> Operand (Simple c) : tokenize cs

implicitConcat :: [Token] -> [Token]
implicitConcat [] = []
implicitConcat [x] = [x]
implicitConcat (x:y:xs) = case (x, y) of
    (Operand op1, Operand op2) -> x : Binary ':' : implicitConcat (y:xs)
    (Operand op1, OpenParen)   -> x : Binary ':' : y : implicitConcat xs
    (CloseParen, Operand op2)  -> x : Binary ':' : implicitConcat (y:xs)

    (Unary op1, Operand op2)   -> x : Binary ':' : implicitConcat (y:xs)
    (Unary op1, OpenParen)     -> x : Binary ':' : y : implicitConcat xs

    (CloseParen, OpenParen)    -> x : Binary ':' : y : implicitConcat xs

    (_, _) -> x : implicitConcat (y:xs)
-----------------------------------------------------------
