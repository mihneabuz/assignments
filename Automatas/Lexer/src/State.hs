module State where

--------------------------IMPORTS--------------------------
import Data.List
-----------------------------------------------------------


-----------------------DECLARATION------------------------ 
data State = Sink | SimpleState Int | MultiState [Int]

instance Ord State where
  compare Sink Sink = EQ
  compare Sink _ = GT
  compare _ Sink = LT
  compare (SimpleState a) (SimpleState b) = compare a b
  compare (MultiState a) (MultiState b) = compare a b
  compare (SimpleState _) (MultiState _) = GT
  compare (MultiState _) (SimpleState _) = LT

instance Eq State where
  Sink == Sink = True
  (SimpleState a) == (SimpleState b) = a == b
  (MultiState a) == (MultiState b) = a == b
  _ == _ = False

instance Read State where
  readsPrec a str = do
    (lexem, rest) <- lex str
    return (SimpleState (read lexem), rest)

instance Show State where
    show (SimpleState x) = show x
    show (MultiState x)  = show x
    show Sink = "sink"
-----------------------------------------------------------


--------------------------FUNCTIONS------------------------ 
makeState :: [Int] -> State
makeState [] = Sink
makeState [x] = SimpleState x
makeState xs  = MultiState xs

combineStates :: [State] -> State
combineStates [] = Sink
combineStates [s] = s
combineStates states = MultiState . map head . group . sort . concatMap extract $ states
    where extract (SimpleState x) = [x]
          extract (MultiState x) = x

subsetOf :: State -> State -> Bool
subsetOf Sink _ = False
subsetOf _ Sink = False
subsetOf (SimpleState s1) (SimpleState s2) = s1 == s2
subsetOf (SimpleState _) _ = False
subsetOf (MultiState xs) (SimpleState x) = x `elem` xs
-----------------------------------------------------------
