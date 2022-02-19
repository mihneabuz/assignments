import qualified Tester as TesterEtapa1

import Lexer
import Parser

import Data.List (sort, sortBy, intercalate)
import System.Process
import System.Directory

main :: IO ()
main = do
    TesterEtapa1.runAll 1
    callProcess "test/T2/runTests.sh" []
    runAllLexerTests "test/T3/tests/T3/regex"
    runAllParserTests "test/T3/tests/T3/prog"

runAllLexerTests :: FilePath -> IO ()
runAllLexerTests testsDir = do
    putStrLn "################ LEXER ##################"
    tests <- getDirectoryContents testsDir
    mapM_ (runLexerTests testsDir) . sortBy _comp . drop 2 $ tests
        where _comp s1 s2 = compare (read (drop 3 s1) :: Int) (read (drop 3 s2) :: Int)

runLexerTests :: FilePath -> FilePath -> IO ()
runLexerTests testsDir test = do
    let thisDir = testsDir ++ "/" ++ test
    let specFile = thisDir ++ "/" ++ test ++ ".lex"
    parser <- fromRegexSpecFile specFile

    let inputDir = thisDir ++ "/input"
    inputFiles <- getDirectoryContents inputDir

    putStrLn $ "######## " ++ test ++ " ########"
    mapM_ (
            \file -> let inputFile = thisDir ++ "/input/" ++ file in
                     let refFile = thisDir ++ "/ref/" ++ (init . init $ file) ++ "ref" in
                         _runLexerTest parser inputFile refFile
          ) $ drop 2 inputFiles

_runLexerTest :: Lexer -> FilePath -> FilePath -> IO ()
_runLexerTest parser inputFile refFile = do
    out <- runLexerVerboseOnFile parser inputFile
    ref <- readFile refFile
    putStrLn $ if out == ref then "PASSED" else "FAIL"

runAllParserTests :: FilePath -> IO ()
runAllParserTests testsDir = do
    putStrLn "################ PARSER #################"
    tests <- getDirectoryContents (testsDir ++ "/input")
    mapM_ (_runParserTest testsDir) . sort . drop 2 $ tests

_runParserTest  :: FilePath -> FilePath -> IO ()
_runParserTest testsDir test = do
    input <- readFile (testsDir ++ "/input/" ++ test)
    output <- readFile (testsDir ++ "/ref/" ++ (init . init $ test) ++ "ref")

    putStrLn test
    let prog = runImperativeParser input
    putStrLn $ if all (uncurry (==)) (zip (showProg prog) output) then "PASSED" else "FAIL"
    mapM_ (putStrLn . \(var, val) -> var ++ " = " ++ show val) $ runImperativeInterpreter prog
    putStrLn ""

    writeFile (testsDir ++ "/out/" ++ (init . init $ test) ++ "out") (showProg prog)
