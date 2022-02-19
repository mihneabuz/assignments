module DFA (
    DFA (DFA),
    createDFA, parseDFA,
    inFinalState, inSinkState,
    step, safeStep, accept
) where

--------------------------IMPORTS--------------------------
import State

import Data.List
import Data.List.Split
import Data.Maybe
import qualified Data.Map as Map
import qualified Data.Set as Set 
-----------------------------------------------------------


-----------------------DECLARATION------------------------ 
type Config = (State, [Char])
type Delta = Map.Map State (Map.Map Char State)
type Alphabet = Set.Set Char
type StateSet = Set.Set State

data DFA = DFA
             String      -- Token
             Alphabet    -- Alphabet
             State       -- Initial State
             Delta       -- Delta Table
             StateSet    -- Final States
             StateSet    -- Sink States

instance Show DFA where
    show dfa@(DFA token alphabet start delta finals sinks) =
        show token ++ "\n" ++ 
        (Set.toList alphabet) ++ "\n" ++
        (show . length . statesDFA $ dfa) ++ "\n" ++
        (show start) ++ "\n" ++
        (foldl (\acc s -> acc ++ show s ++ " ") "" . Set.toList $ finals) ++ "\n" ++
        (intercalate "\n" . map (\(s1, (c, s2)) -> show s1 ++ "," ++ show c ++ "," ++ show s2) $ transitions)
            where transitions = concat . map (\(x, m) -> zip (repeat x) . Map.toList $ m) . Map.toList $ delta
-----------------------------------------------------------


-----------------------CONSTRUCTION------------------------ 
createDFA :: String -> [Char] -> State -> [(State, Char, State)] -> [State] -> DFA
createDFA token chars start trans final = simplifyStates (DFA token alphabet start transitions finals sinks)
    where 
        alphabet = Set.fromList chars
        finals = Set.fromList final
        sinks = Set.singleton Sink
        transitions = foldl addTransition Map.empty trans

        addTransition :: Delta -> (State, Char, State) -> Delta
        addTransition delta (from, char, to) = Map.insert from updatedTrans delta
            where updatedTrans = case Map.lookup from delta of
                    Just oldTrans -> Map.insert char to oldTrans
                    Nothing -> Map.singleton char to

simplifyStates :: DFA -> DFA
simplifyStates dfa@(DFA token alphabet start delta finals sinks) = DFA token alphabet newStart newDelta newFinals newSinks
    where 
        assocs = Map.fromList . zip (statesDFA dfa) . map (\x -> makeState [x]) $ [0..]
        newState s = assocs Map.! s 

        newStart = newState start
        newFinals = Set.map newState finals
        newDelta = Map.map (Map.map newState) . Map.mapKeys newState $ delta
        newSinks = Set.map newState sinks
-----------------------------------------------------------


-----------------------INPUT PARSING----------------------- 
parseDFA :: [Char] -> DFA
parseDFA str = DFA token alphabet start delta finals (findSinkStates delta finals)
    where 
        strLines = tail . lines $ str
        token = head strLines
        start = read . head . tail $ strLines
        finals = Set.fromList . map read . words . last $ strLines
        transitions = init . tail . tail $ strLines
        (alphabet, delta) = parseTransitions transitions

parseTransitions :: [String] -> (Alphabet, Delta)
parseTransitions x = parseTransHelper x Set.empty Map.empty
    where
        parseTransHelper :: [String] -> Alphabet -> Delta -> (Alphabet, Delta)
        parseTransHelper [] alphabet delta = (alphabet, delta)
        parseTransHelper (x:xs) alphabet delta = parseTransHelper xs updatedAlphabet updatedDelta
            where
                [fromStateVal, charVal, toStateVal] = splitOn "," x
                char      = read charVal       :: Char
                fromState = read fromStateVal  :: State
                toState   = read toStateVal    :: State

                updatedTrans = case Map.lookup fromState delta of
                  Just oldTrans -> Map.insert char toState oldTrans
                  Nothing -> Map.singleton char toState

                updatedAlphabet = Set.insert char alphabet
                updatedDelta = Map.insert fromState updatedTrans delta

findSinkStates :: Delta -> StateSet -> StateSet
findSinkStates delta finals = Set.fromList sinks
    where
        states = Map.keys delta
        sinks = filter (\state -> not $ canReachFinal state delta finals Set.empty) states

        canReachFinal :: State -> Delta -> StateSet -> StateSet -> Bool
        canReachFinal currState delta finals seen = Set.member currState finals || recurse
            where
                notSeenStates = filter (\state -> not (Set.member state seen)) adjStates
                adjStates = maybe [] Map.elems (Map.lookup currState delta)
                recurse = any (\state -> canReachFinal state delta finals (Set.insert state seen)) notSeenStates 
-----------------------------------------------------------


--------------------------FUNCTIONS------------------------ 
statesDFA :: DFA -> [State]
statesDFA (DFA _ _ start delta finals _) = map head . group . sort $ [start] ++ Set.toList finals ++ Map.keys delta

inSinkState :: DFA -> Config -> Bool
inSinkState (DFA _ _ _ delta _ sinks) (state, _) = Set.member state sinks || isNothing (Map.lookup state delta)

inFinalState :: DFA -> Config -> Bool
inFinalState (DFA _ _ _ _ finals _) (state, _) = Set.member state finals

step :: DFA -> Config -> Maybe Config
step _ (_, []) = Nothing
step (DFA _ alphabet _ delta _ _) (currState, char:rest)
  | not $ Set.member char alphabet = Nothing
  | otherwise = do 
      stateMap <- Map.lookup currState delta
      nextState <- Map.lookup char stateMap
      return (nextState, rest)

safeStep :: DFA -> Config -> Maybe Config
safeStep dfa config = do
    nextConfig <- step dfa config
    if inSinkState dfa nextConfig
       then Nothing
       else return nextConfig

accept :: DFA -> [Char] -> Bool
accept _ [] = False
accept dfa@(DFA _ _ start _ _ _) str = acceptHelper dfa (start, str)
  where 
    acceptHelper :: DFA -> Config -> Bool
    acceptHelper dfa config
      | null . snd $ config = inFinalState dfa config
      | inSinkState dfa config = False
      | otherwise = maybe False (acceptHelper dfa) $ step dfa config
-----------------------------------------------------------


----------------------------DEBUG-------------------------- 
debugDFA :: DFA -> String
debugDFA (DFA token alphabet start delta finals sinks) =
    "Token: " ++ token ++ "\n" ++
    "Alphabet -> " ++ "[" ++ alphabetVal ++ "]\n" ++
    "Initial State -> " ++ show start ++ "\n" ++
    "Final States -> " ++ show (statesValues finals) ++ "\n" ++
    "Sink States -> " ++ show (statesValues sinks) ++ "\n" ++
    "Transitions:\n" ++ transitions
    where
        alphabetVal = Set.toList alphabet
        statesValues = combineStates . Set.toList
        transitions = unlines . map (\(s, map) -> show s ++ ": " ++ showMap map) . Map.toList $ delta
        showMap = unwords . map showMapEntry . Map.toList
        showMapEntry (char, state) = show char ++ " -> " ++ show state
-----------------------------------------------------------
