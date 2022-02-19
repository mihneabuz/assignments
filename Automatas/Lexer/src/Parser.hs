module Parser (runImperativeParser, runImperativeInterpreter, showProg) where

--------------------------IMPORTS--------------------------
import Lexer

import Data.List (intercalate)
import qualified Data.Map as Map
-----------------------------------------------------------


-----------------------DECLARATION------------------------ 
type Tokens = [(String, String)]

data Expr = Variable String
          | Integer Int
          | Operator String Expr Expr
          deriving (Show, Eq)

data Prog = Assignment String Expr
          | If Expr Prog Prog
          | While Expr Prog
          | Secv [Prog] 
          deriving (Show, Eq)

type Asocs = Map.Map String Int
-----------------------------------------------------------


---------------------IMPERATIVE SPEC----------------------- 
keywords   = "KEYWORD begin|end|if'('|')'then|else|fi|while'('|')'do|od;"
variable   = "VARIABLE [a-z]+[0-9]*"
assignment = "ASSIGNMENT =;"
operators  = "OPERATOR '+'|'-''-'|'*'|'>'|'=''=';"
integer    = "INTEGER [0-9]+|'-'[0-9]+;"
linefeed   = "NEWLINE '\\n'*;"

imperativeSpec = intercalate "\n" [ keywords  
                                  , variable  
                                  , assignment
                                  , operators 
                                  , integer   
                                  , linefeed  
                                  ]

imperativeLexer = fromRegexSpec imperativeSpec

precedence "+"  = 1
precedence "--" = 1
precedence "*"  = 2
precedence ">"  = 3
precedence "==" = 3
precedence _    = error "BAD OPERATOR"

operatorName "+"  = "plus"
operatorName "--" = "minus"
operatorName "*"  = "multiply"
operatorName ">"  = "greaterthan"
operatorName "==" = "equals"
operatorName _    = error "BAD OPERATOR"
-----------------------------------------------------------


--------------------------FUNCTIONS------------------------ 
runImperativeParser :: String -> Prog
runImperativeParser = parseProgram . runLexer imperativeLexer . strip
    where 
        strip [] = []
        strip ('-':' ':xs) = "--" ++ strip xs
        strip (' ':xs)     = strip xs
        strip (x:xs)       = x : strip xs 

runImperativeInterpreter :: Prog -> [(String, Int)]
runImperativeInterpreter = Map.toList . evalProg Map.empty
    where
        evalProg :: Asocs -> Prog -> Asocs
        evalProg asocs (Assignment var expr) = Map.insert var value asocs
            where value = evalExpr asocs expr
    
        evalProg asocs (If expr true false) = if value == 0 then evalProg asocs false
                                                            else evalProg asocs true
            where value = evalExpr asocs expr

        evalProg asocs (While expr prog) = if value == 0 then asocs
                                                         else evalProg newAssocs (While expr prog)
            where value = evalExpr asocs expr
                  newAssocs = evalProg asocs prog

        evalProg asocs (Secv progs) = foldl evalProg asocs progs

evalExpr :: Asocs -> Expr -> Int
evalExpr _ (Integer int) = int
evalExpr asocs (Operator op expr1 expr2) = case op of
    "+"  -> int1 + int2     
    "--" -> int1 - int2
    "*"  -> int1 * int2
    ">"  -> if int1 >  int2 then 1 else 0
    "==" -> if int1 == int2 then 1 else 0
    where int1 = evalExpr asocs expr1
          int2 = evalExpr asocs expr2
evalExpr asocs (Variable var) = case Map.lookup var asocs of
    Just int -> int
    Nothing  -> error $ "VARIABLE '" ++ var ++ "' NOT DEFINED!\n"

showProg :: Prog -> String
showProg = _showProg 0
    where
        _showProg depth (Secv progs) = 
            spaces (2 * depth) ++ "[\n" ++ concatMap (_showProg (depth + 1)) progs ++ spaces (2 * depth) ++ "]\n"

        _showProg depth (Assignment var expr) = 
            spaces (2 * depth) ++ "assign [\n" ++ 
                _showExpr (depth + 1) (Variable var) ++ 
                _showExpr (depth + 1) expr ++
                    spaces (2 * depth) ++ "]\n"

        _showProg depth (If expr prog1 prog2) = 
            spaces (2 * depth) ++ "if [\n" ++ 
                _showExpr (depth + 1) expr ++
                spaces (2 * depth + 2) ++ "then " ++ drop (2 * depth + 2) (_showProg (depth + 1) prog1) ++
                spaces (2 * depth + 2) ++ "else " ++ drop (2 * depth + 2) (_showProg (depth + 1) prog2) ++
                    spaces (2 * depth) ++ "]\n"

        _showProg depth (While expr prog) =
            spaces (2 * depth) ++ "while [\n" ++ 
                _showExpr (depth + 1) expr ++
                spaces (2 * depth + 2) ++ "do " ++ drop (2 * depth + 2) (_showProg (depth + 1) prog) ++
                    spaces (2 * depth) ++ "]\n"

        _showExpr depth (Variable str) = 
            spaces (2 * depth) ++ "variable [\n" ++ spaces (2 * depth + 2) ++ str ++ "\n" ++ spaces (2 * depth) ++ "]\n"

        _showExpr depth (Integer int)  = 
            spaces (2 * depth) ++ "integer [\n" ++ spaces (2 * depth + 2) ++ show int ++ "\n" ++ spaces (2 * depth) ++ "]\n"

        _showExpr depth (Operator op expr1 expr2) =
            spaces (2 * depth) ++ operatorName op ++ " [\n" ++ 
                _showExpr (depth + 1) expr1 ++ 
                _showExpr (depth + 1) expr2 ++ 
                    spaces (2 * depth) ++ "]\n"

parseProgram :: [(String, String)] -> Prog
parseProgram = fst . _parse
    where
        _parse :: Tokens -> (Prog, Tokens)
        _parse ((token, lexem):tokens) = case (token, lexem) of
            ("NEWLINE", _) -> _parse tokens

            ("VARIABLE", _) -> (Assignment lexem expr, rest)
                where (expr, rest) = _parseAssignment tokens
            
            ("KEYWORD", "begin") -> (Secv progs, rest)
                where (progs, rest) = _parseSecv tokens []

            ("KEYWORD", "if(") -> (If expr true false, rest)
                where (expr, true, false, rest) = _parseIf tokens

            ("KEYWORD", "while(") -> (While expr loop, rest)
                where (expr, loop, rest) = _parseWhile tokens

            _ -> error $ "BAD PARSE: " ++ show token ++ " " ++ show lexem

        _parseAssignment tokens = case head tokens of 
            ("ASSIGNMENT", "=") -> (parseExpression exprTokens, tail rest)
                where (exprTokens, rest) = span ((/= "NEWLINE") . fst) tokens
            _ -> error $ "BAD ASSIGNMENT: " ++ show (head tokens)

        _parseIf tokens = case head rest3 of
            ("KEYWORD", "fi") -> (expr, true, false, tail rest3)
            _ -> error $ "BAD IF" ++ show rest3
            where 
                (exprTokens, branchTokens) = span (/= ("KEYWORD", ")then")) tokens
                expr = parseExpression exprTokens
                (true, rest1) = _parse . tail $ branchTokens
                (false, rest2) = _parse . tail $ rest1
                rest3 = dropWhile ((== "NEWLINE") . fst) rest2

        _parseWhile tokens = case head rest2 of
            ("KEYWORD", "od") -> (expr, loop, tail rest2)
            _ -> error $ "BAD WHILE" ++ show rest2
            where 
                (exprTokens, branchTokens) = span (/= ("KEYWORD", ")do")) tokens
                expr = parseExpression exprTokens
                (loop, rest1) = _parse . tail $ branchTokens
                rest2 = dropWhile ((== "NEWLINE") . fst) rest1

        _parseSecv ((token, lexem):tokens) progs = case (token, lexem) of
            ("KEYWORD", "end") -> (reverse progs, tokens)
            ("NEWLINE", _)     -> _parseSecv tokens progs
            _                  -> _parseSecv rest (newProg:progs)
                where (newProg, rest) = _parse ((token, lexem):tokens)

parseExpression :: Tokens -> Expr
parseExpression = fst . _parseExpr . reverse . toPostfix 
    where
        _parseExpr ((token, lexem):tokens) = case token of
            "VARIABLE" -> (Variable lexem, tokens)
            "INTEGER"  -> (Integer (read lexem), tokens)
            "OPERATOR" -> (Operator lexem expr2 expr1, tokens2)
                where (expr1, tokens1) = _parseExpr tokens
                      (expr2, tokens2) = _parseExpr tokens1
            _ -> error $ "BAD EXPRESSION TOKEN: " ++ show token ++ " " ++ show lexem
-----------------------------------------------------------


---------------------------HELPERS------------------------- 
spaces :: Int -> String
spaces x = replicate x ' '

toPostfix :: Tokens -> Tokens
toPostfix = flip _toPostfix []
    where
        _toPostfix :: Tokens -> Tokens -> Tokens
        _toPostfix [] stack = stack
        _toPostfix (token@("OPERATOR", operator):rest) stack = before ++ _toPostfix rest (token:after)
            where (before, after) = span (\(_, x) -> precedence operator > precedence x) stack
        _toPostfix (token:rest) stack = token : _toPostfix rest stack
-----------------------------------------------------------
