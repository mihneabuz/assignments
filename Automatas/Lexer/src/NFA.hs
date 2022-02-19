module NFA (NFA (NFA), Transition (Epsilon, Normal),
            completeNFA, toDFA, toDFAWithName
           ) where

--------------------------IMPORTS--------------------------
import State
import DFA

import Data.List
import qualified Data.Map as Map
-----------------------------------------------------------


-----------------------DECLARATION------------------------ 
data NFA = NFA State State [Transition]

instance Show NFA where
    show (NFA start final transitions) =
        show start ++ "\n" ++
        show final ++ "\n" ++
        (intercalate "\n" . map show $ transitions)

getTrans :: NFA -> [Transition]
getTrans (NFA _ _ trans) = trans

data Transition = Epsilon State State | Normal State Char State

instance Show Transition where
    show (Epsilon x y) = show x ++ " -> Eps -> " ++ show y 
    show (Normal x c y) = show x ++ " -> " ++ show c ++ " -> " ++ show y

epsilonTrans :: Transition -> Bool
epsilonTrans (Epsilon _ _) = True
epsilonTrans _ = False

from :: Transition -> State
from (Epsilon s _) = s
from (Normal s _ _) = s

char :: Transition -> Char
char (Epsilon _ _) = '`'
char (Normal _ c _) = c

to :: Transition -> State
to (Epsilon _ s) = s
to (Normal _ _ s) = s
-----------------------------------------------------------


------------------------FUNCTIONS-------------------------- 
completeNFA :: NFA -> NFA
completeNFA nfa@(NFA start final trans) = completeNFA' nfa initialNFA
    where
        initalState = combineStates . epsilonStates nfa $ start
        initialNFA = NFA initalState Sink (findTransitions nfa initalState)

        completeNFA' :: NFA -> NFA -> NFA       
        completeNFA' baseNFA partialNFA@(NFA _ _ trans) = if isDFA partialNFA then partialNFA else recurse
            where incomplete = incompleteStates partialNFA
                  nextPartialNFA = addTransitions partialNFA . concatMap (findTransitions baseNFA) $ incomplete
                  recurse = completeNFA' baseNFA nextPartialNFA

toDFA :: NFA -> DFA
toDFA = toDFAWithName "fromNFA"

toDFAWithName :: String -> NFA -> DFA
toDFAWithName token nfa@(NFA _ oldFinal _) = convertToDFA completedNFA finalStates
    where 
        completedNFA = completeNFA nfa
        finalStates = filter (`subsetOf` oldFinal) . statesNFA $ completedNFA

        convertToDFA :: NFA -> [State] -> DFA
        convertToDFA nfa@(NFA start _ trans) final = createDFA token alphabet start transitions final
            where alphabet = alphabetNFA nfa
                  transitions = map (\t -> (from t, char t, to t)) trans
-----------------------------------------------------------


-------------------------HELPERS--------------------------- 
transStates :: Transition -> [State]
transStates (Epsilon s1 s2) = [s1, s2]
transStates (Normal s1 _ s2) = [s1, s2]

uniques :: Ord a => [a] -> [a]
uniques = map head . group . sort

alphabetNFA :: NFA -> [Char]
alphabetNFA (NFA _ _ trans) = uniques . map (\(Normal _ c _) -> c) . filter (not . epsilonTrans) $ trans

statesNFA :: NFA -> [State]
statesNFA (NFA s1 s2 trans) = uniques . sort $ allStates
    where allStates = [s1, s2] ++ concatMap transStates trans

addTransitions :: NFA -> [Transition] -> NFA
addTransitions (NFA start final trans) newTrans = NFA start final (newTrans ++ trans)

isDFA :: NFA -> Bool
isDFA nfa@(NFA start final trans) = (not . any epsilonTrans $ trans) && (null . incompleteStates $ nfa)

incompleteStates :: NFA -> [State]
incompleteStates nfa@(NFA start final trans) = filter (not . stateComplete) . statesNFA $ nfa
    where stateComplete state = all (transitionExists state) . alphabetNFA $ nfa
          transitionExists s1 c = length (filter (\t -> from t == s1 && char t == c) trans) == 1

epsilonStates :: NFA -> State -> [State]
epsilonStates (NFA _ _ trans) s = epsilonStates' [s]
    where
        epsilonStates' :: [State] -> [State]
        epsilonStates' states = if null nextStates then states else epsilonStates' . uniques $ (nextStates ++ states)
            where nextStates = filter notDups . map to . filter fromStates . filter epsilonTrans $ trans
                  notDups = not . flip elem states
                  fromStates = flip elem states . from

findTransitions :: NFA -> State -> [Transition]
findTransitions nfa s = map (\c -> Normal s c (reachable s c)) . alphabetNFA $ nfa
    where
        normalTrans = filter (not . epsilonTrans) $ getTrans nfa
        combineEpsilon = combineStates . uniques . concatMap (epsilonStates nfa)

        reachable :: State -> Char -> State
        reachable s@(SimpleState _) c = combineEpsilon . map to . filter (\t -> from t == s && char t == c) $ normalTrans
        reachable s@(MultiState _) c = combineEpsilon . map to . filter (\t -> subsetOf s (from t) && char t == c) $ normalTrans
        reachable Sink c = Sink
-----------------------------------------------------------
