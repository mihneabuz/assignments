{-# LANGUAGE ExistentialQuantification #-}
{-# LANGUAGE FlexibleInstances #-}
{-
	PP Project 2021

	This is where you will write the implementation for the given tasks.
	You can add other modules aswell.
-}

module Tasks where 
import Dataset
import Text.Printf
import Data.Array

type CSV = String
type Value = String
type Row = [Value]
type Table = [Row]

{-
	TASK SET 1
-}

-- for easier string conversion
rfl :: Value -> Float
rfl x 
  | x == "" = 0
  | otherwise = read x :: Float

rint :: Value -> Int
rint x 
  | x == "" = 0
  | otherwise = read x :: Int

show' :: Float -> Value
show' = printf "%.2f"

-- Task 1
-- change header, use map to compute grade
compute_exam_grades :: Table -> Table
compute_exam_grades (header:xs) = ((header !! 0) : "Punctaj Exam" : []) : (map (compute) xs)
  where 
    compute (nume:q1:q2:q3:q4:q5:q6:ex:_) = nume : (show' $ sum / 4 + exam) : []
      where sum = rfl q1 + rfl q2 + rfl q3 + rfl q4 + rfl q5 + rfl q6
            exam = rfl ex

-- Task 2
-- Number of students who have passed the exam:
-- filter passed and count
get_passed_students_num :: Table -> Int
get_passed_students_num = length . filter (passed) . tail . compute_exam_grades 
  where passed (_:grade:_) = rfl grade > 2.5

-- Percentage of students who have passed the exam:
-- divide passed by num students
get_passed_students_percentage :: Table -> Float
get_passed_students_percentage x = num_passed / num_students
  where num_students = fromIntegral (length x - 1) :: Float
        num_passed = fromIntegral (get_passed_students_num x) :: Float

-- Average exam grade
-- fold over data and calculate both sum and count, divide at the end
get_exam_avg :: Table -> Float
get_exam_avg = avg . foldl (count_and_sum) (0, 0) . tail . compute_exam_grades
  where count_and_sum = \(sum, count) (_:grade:_) -> (sum + rfl grade, count + 1)
        avg (sum, count) = sum / fromIntegral count :: Float

-- Number of students who gained at least 1.5p from homework:
-- use map to get grade for each then filter and count
get_passed_hw_num :: Table -> Int
get_passed_hw_num = length . filter (>=1.5) . map (hwTotal) . tail
  where hwTotal (name:l:t1:t2:t3:_) = rfl t1 + rfl t2 + rfl t3

-- Task 3
-- using this to fold over data
sum_row :: [Float] -> [Float] -> [Float]
sum_row [] [] = []
sum_row (a:ax) (b:bx) = (a + b) : (sum_row ax bx)

-- convert to floats, fold over data summing all columns and compute average
get_avg_responses_per_qs :: Table -> Table
get_avg_responses_per_qs grades = table . foldl (sum_row) zeros . map (questions) . tail $ grades
  where 
    zeros = [0,0,0,0,0,0] 
    questions (name:q1:q2:q3:q4:q5:q6:_) = [rfl q1, rfl q2, rfl q3, rfl q4, rfl q5, rfl q6]
    table x = ["Q1", "Q2", "Q3", "Q4", "Q5", "Q6"] : (map (avg) x) : []
      where avg x = show' $ x / (fromIntegral (length grades - 1) :: Float)
        

-- Task 4

-- transpose
tr :: [[a]] -> [[a]]
tr ([]:_) = []
tr x = (map head x) : tr (map tail x)

-- use map to convert to Int, transpose, count for each question and make table
get_exam_summary :: Table -> Table
get_exam_summary = table . map (count) . tr . map (questions) . tail  
  where 
    questions (name:q1:q2:q3:q4:q5:q6:_) = [rint q1, rint q2, rint q3, rint q4, rint q5, rint q6]
    count = foldl (match) [0, 0, 0]
      where match (x0:x1:x2:_) x
              | x == 0 = ((x0 + 1):x1:x2:[])
              | x == 1 = (x0:(x1 + 1):x2:[])
              | x == 2 = (x0:x1:(x2 + 1):[])
    -- add header, and prepend rows with index using zip and map
    table counts = ["Q", "0", "1", "2"] : (map (parse) . zip [1..6] $ counts)
      where parse = \(i, l) -> ("Q" ++ (show i)) : (map (show) l)

-- Task 5
-- compare two students with Name:Grade:[]
compare_by_grade :: Row -> Row -> Ordering
compare_by_grade (s1:grade1:_) (s2:grade2:_)
  | rfl grade1 == rfl grade2 = compare s1 s2
  | rfl grade1 > rfl grade2 = GT
  | otherwise = LT

-- quicksort by compare func
sort_students :: (Row -> Row -> Ordering) -> Table -> Table
sort_students comp [] = []
sort_students comp (x:xs) =
  let smaller = sort_students comp [a | a <- xs, comp a x == LT]
      bigger = sort_students comp [a | a <- xs, comp a x == GT || comp a x == EQ]
  in smaller ++ [x] ++ bigger

-- compute exam grades, remove header, sort and put header back
get_ranking :: Table -> Table
get_ranking table = (head grades) : (sort_students (compare_by_grade) . tail $ grades)
  where grades = compute_exam_grades table

-- Task 6
-- compare students with Nume:Interviu:Scris:Diff
compare_by_diff :: Row -> Row -> Ordering
compare_by_diff (s1:_:_:diff1:_) (s2:_:_:diff2:_)
  | rfl diff1 == rfl diff2 = compare s1 s2
  | rfl diff1 > rfl diff2 = GT
  | otherwise = LT

-- remove header, use map to calculate diff, sort by diff and add header
get_exam_diff_table :: Table -> Table
get_exam_diff_table = header . sort_students (compare_by_diff) . map (compute) . tail
  where 
    compute (name:q1:q2:q3:q4:q5:q6:ex:_) = name : (show' qs) : (show' . rfl $ ex) : (diff qs ex) : []
      where qs = (rfl q1 + rfl q2 + rfl q3 + rfl q4 + rfl q5 + rfl q6) / 4
            diff qs ex = show' . abs $ rfl ex - qs
    header table = ["Nume", "Punctaj interviu", "Punctaj scris", "Diferenta"] : table



{-
	TASK SET 2
-}

-- Task 0
-- split by \n then split by ,
read_csv :: CSV -> Table
read_csv = map (splitBy ',') . splitBy '\n' 

splitBy :: Char -> String -> [String]
splitBy delim s = foldr (split) [""] s
  where
    split x acc@(a:as)
      | x == delim = "" : acc
      | otherwise = ((x:a):as)

-- join by , then join by \n
write_csv :: Table -> CSV
write_csv = joinBy '\n' . map (joinBy ',') 

joinBy :: Char -> [String] -> String
joinBy _ [] = []
joinBy _ (x:[]) = x
joinBy delim (x:xs) = x ++ [delim] ++ (joinBy delim xs)

-- Task 1
-- find column index, map
as_list :: String -> Table -> [String]
as_list column t = map (!! c) . tail $ t
  where c = find' column . head $ t

find' :: Eq a => a -> [a] -> Int
find' e l = helper e l 0
  where
    helper _ [] count = 0
    helper e (x:xs) count 
      | x /= e = helper e xs (count + 1)
      | otherwise = count

-- Task 2
-- find column index, compare by column value, sort (from last task set)
tsort :: String -> Table -> Table
tsort column t = (head t) : (sort_students (compare_by_col c) . tail $ t)
  where c = find' column . head $ t

-- check if value is numeric
is_numeric :: Value -> Bool
is_numeric [] = True
is_numeric (x:xs) = if (x >= '0' && x <= '9') || x == '.' then True else False

compare_by_col :: Int -> Row -> Row -> Ordering
compare_by_col col student1 student2
  | student1 !! col == student2 !! col = compare (student1 !! 0) (student2 !! 0)
  | student1 !! col == "" || student2 !! col == "" = compare (student1 !! col) (student2 !! col)
  | is_numeric (student1 !! col) = compare (rfl (student1 !! col)) (rfl (student2 !! col))
  | otherwise = compare ((student1 !! col)) ((student2 !! col))

-- Task 3
-- map
vmap :: (Value -> Value) -> Table -> Table
vmap _ [] = []
vmap f (x:xs) = (map f x) : (vmap f xs)

-- Task 4
-- map
rmap :: (Row -> Row) -> [String] -> Table -> Table
rmap f header t = header : map f (tail t)

get_hw_grade_total :: Row -> Row
get_hw_grade_total row = (head row) : (show' . sum . map (rfl) . hw $ row) : []
  where hw = tail . tail

-- Task 5
-- join if headers are same
vunion :: Table -> Table -> Table
vunion t1 t2 
  | head t1 == head t2 = t1 ++ tail t2
  | otherwise = t1

-- Task 6
-- zip with join
hunion :: Table -> Table -> Table
hunion = zipLongest (\x y -> x ++ y)

zipLongest :: (Row -> Row -> Row) -> Table -> Table -> Table
zipLongest _ [] [] = []
zipLongest f (x:xs) [] = f x (fill x) : zipLongest f [] xs
zipLongest f [] (y:ys) = f y (fill y) : zipLongest f [] ys
zipLongest f (x:xs) (y:ys) = (f x y) : zipLongest f xs ys

fill :: Row -> Row
fill x = take (length x - 1) . repeat $ ""

-- Task 7
-- find columns, merge headers, map merge rows
tjoin :: String -> Table -> Table -> Table
tjoin column t1 t2 = header : (map (merge) . tail $ t1)
  where 
    c1 = find' column . head $ t1
    c2 = find' column . head $ t2
    header = head t1 ++ (drop (c2 + 1) . head $ t2)
    merge row1 = row1 ++ (drop (c2 + 1) row2)
      where 
        found = find_row (row1 !! c1) c2 (tail t2)
        row2 = if (found == []) then fill row1 else found

-- finds row with value val on column col
find_row :: String -> Int -> Table -> Row
find_row _ _ [] = []
find_row val col (x:xs)
  | (x !! col) == val = x
  | otherwise = find_row val col xs


-- Task 8
cartesian :: (Row -> Row -> Row) -> [String] -> Table -> Table -> Table
cartesian f columns t1 t2 = columns : helper f (tail t1) (tail t2)
  where
    helper _ [] _ = []
    helper f (x:xs) [] = helper f xs (tail t2)
    helper f (x:xs) (y:ys) = (f x y) : helper f (x:xs) ys

-- Task 9
-- fold, transpose and add header
projection :: [String] -> Table -> Table
projection columns t = columns : (tr $ foldr (join) [] columns)
  where join col acc = (as_list col t) : acc


{-
	TASK SET 3
-}

data Query =
    FromCSV CSV
    | ToCSV Query
    | AsList String Query
    | Sort String Query
    | ValueMap (Value -> Value) Query
    | RowMap (Row -> Row) [String] Query
    | VUnion Query Query
    | HUnion Query Query
    | TableJoin String Query Query
    | Cartesian (Row -> Row -> Row) [String] Query Query
    | Projection [String] Query
    | forall a. FEval a => Filter (FilterCondition a) Query
    | Graph EdgeOp Query

similarities_op :: EdgeOp
similarities_op row1 row2 = if s >= 5 then Just (show s) else Nothing
  where
    s = sum $ zipWith (comp) (tail row1) (tail row2)
    comp x y = if x == y then 1 else 0

no_empy_emails = FNot (Eq "Email" "")

similarities_query = 
    Sort "Value" $
      Graph similarities_op $
        Filter no_empy_emails $
          FromCSV lecture_grades_csv
 
-- where EdgeOp is defined:
type EdgeOp = Row -> Row -> Maybe Value 

data QResult = CSV CSV | Table Table | List [String]
-- don't confuse first 'CSV' and second 'CSV': first refers to constructor name,
-- second refers to type CSV (defined in taskset 1); same with 'Table'.

instance Show QResult where
  show (CSV csv) = show csv
  show (Table t) = write_csv t
  show (List l) = show l

class Eval a where
    eval :: a -> QResult
 
instance Eval Query where
  eval (FromCSV str) = Table (read_csv str)

  eval (ToCSV query) = CSV (write_csv result)
    where (Table result) = eval query 

  eval (AsList colname query) = List (as_list colname result)
    where (Table result) = eval query

  eval (Sort colname query) = Table (tsort colname result)
    where (Table result) = eval query
 
  eval (ValueMap op query) = Table (vmap op result)
    where (Table result) = eval query
 
  eval (RowMap op header query) = Table (rmap op header result)
    where (Table result) = eval query
 
  eval (VUnion query1 query2) = Table (vunion result1 result2)
    where 
      (Table result1) = eval query1
      (Table result2) = eval query2
  
  eval (HUnion query1 query2) = Table (hunion result1 result2)
    where 
      (Table result1) = eval query1
      (Table result2) = eval query2

  eval (TableJoin header query1 query2) = Table (tjoin header result1 result2)
    where 
      (Table result1) = eval query1
      (Table result2) = eval query2

  eval (Cartesian op header query1 query2) = Table (cartesian op header result1 result2)
    where 
      (Table result1) = eval query1
      (Table result2) = eval query2
      
  eval (Projection header query) = Table (projection header result)
    where (Table result) = eval query

  eval (Filter fcondition query) = Table (tfilter filter result)
    where 
      (Table result) = eval query
      filter = feval (head result) fcondition
  
  eval (Graph op query) = Table (make_graph op result)
    where (Table result) = eval query
    

tfilter :: FilterOp -> Table -> Table
tfilter op t = (head t) : filter op (tail t)

make_edge :: [Row] -> EdgeOp -> Row -> Row -> [Row]
make_edge acc op row1 row2
  | op row1 row2 == Nothing = acc
  | otherwise = [min first second, max first second, value] : acc
    where
      first = row1 !! 0
      second = row2 !! 0
      (Just value) = op row1 row2

make_graph :: EdgeOp -> Table -> Table
make_graph op t = header : helper (tail t)
  where
    header = ["From", "To", "Value"]
    helper [] = []
    helper (x:xs) = (foldr (\row acc -> make_edge acc op x row) [] xs) ++ (helper xs)

data FilterCondition a =
    Eq String a |
    Lt String a |
    Gt String a |
    In String [a] |
    FNot (FilterCondition a) |
    FieldEq String String

type FilterOp = Row -> Bool

class FEval a where
    feval :: [String] -> (FilterCondition a) -> FilterOp

-- helper func for feval, compares two values from a row
_cmp_cols :: [String] -> String -> String -> Row -> Bool
_cmp_cols header col1 col2 row = (row !! i1) == (row !! i2)
  where
    i1 = find' col1 header
    i2 = find' col2 header

instance FEval Float where
  feval header (Eq colname ref) = (== ref) . rfl . (!! (find' colname header))
  feval header (Lt colname ref) = (< ref) . rfl . (!! (find' colname header))
  feval header (Gt colname ref) = (> ref) . rfl . (!! (find' colname header))
  feval header (In colname list) = (flip elem list) . rfl . (!! (find' colname header))
  feval header (FNot condition) = not . (feval header condition)
  feval header (FieldEq col1 col2) = _cmp_cols header col1 col2

instance FEval String where
  feval header (Eq colname ref) = (== EQ) . (compare ref) . (!! (find' colname header))
  feval header (Lt colname ref) = (== GT) . (compare ref) . (!! (find' colname header))
  feval header (Gt colname ref) = (== LT) . (compare ref) . (!! (find' colname header))
  feval header (In colname list) = (flip elem list) . (!! (find' colname header))
  feval header (FNot condition) = not . (feval header condition)
  feval header (FieldEq col1 col2) = _cmp_cols header col1 col2


{-
	TASK SET 4
-}

correct_table :: String -> CSV -> CSV -> CSV
correct_table column table_cvs ref_csv = write_csv (header : fixed_table)
  where
    table = read_csv table_cvs
    ref = read_csv ref_csv

    header = head table
    col = find' column . head $ table
   
    values = as_list column table
    refs = as_list column ref
    maps = map (fst . best_match refs) values

    fixed_table = zipWith (fix_entry col) (tail table) maps 

-- find best match from refs
best_match :: [Value] -> Value -> (Value, Int)
best_match refs val = foldl (helper) (val, 100) refs
  where  
    helper :: (Value, Int) -> Value -> (Value, Int)
    helper (best, dist) ref = if d < dist then (ref, d) else (best, dist)
      where d = editDistance val ref

-- fix entry with new value
fix_entry :: Int -> Row -> Value -> Row
fix_entry col [] val = []
fix_entry col (x:xs) val
  | col == 0 = val : xs
  | otherwise = fix_entry (col - 1) xs val

-- https://wiki.haskell.org/Edit_distance
editDistance :: Eq a => [a] -> [a] -> Int
editDistance xs ys = table ! (m,n)
  where
    (m, n) = (length xs, length ys)
    x = array (1, m) (zip [1..] xs)
    y = array (1, n) (zip [1..] ys)
    
    table :: Array (Int, Int) Int
    table = array bnds [(ij, dist ij) | ij <- range bnds]
    bnds  = ((0, 0), (m, n))
    
    dist (0, j) = j
    dist (i, 0) = i
    dist (i, j) = minimum [table ! (i-1, j) + 1, table ! (i, j-1) + 1,
        if x ! i == y ! j then table ! (i-1, j-1) else 1 + table ! (i-1, j-1)]

calc_hw :: Row -> Row
calc_hw row = (head row) : (show' . sum . map (rfl) . tail $ row) : []

calc_exam :: Row -> Row
calc_exam row = (head row) : (show' total) : []
  where total = (sum . map (rfl) . tail . init $ row) / 4 + (rfl . last $ row)

calc_lec :: Row -> Row
calc_lec row = (head row) : (show' . op . sum . map (rfl) . tail $ row) : []
  where op x = x * 2 / fromIntegral (length row - 1)

calc_final :: Row -> Row
calc_final row = (head row) : (row !! 2) : (row !! 3) : (row !! 4) : [show' final]
  where
    hw = rfl (row !! 2)
    lec = rfl (row !! 3)
    exam = rfl (row !! 4)
    final = if (hw + lec < 2.5) || (exam < 2.5) then 4.0 else (min (hw + lec) 5) + exam

grades :: CSV -> CSV -> CSV -> CSV -> CSV
grades map_csv hw_csv exam_csv lec_csv = write_csv (tsort "Nume" final)
  where
    (hw, exam, lec) = (read_csv hw_csv, read_csv exam_csv, read_csv lec_csv)
    email_map = read_csv (correct_table "Nume" map_csv hw_csv)

    hw_calculated = rmap calc_hw ["Nume", "Punctaj teme"] hw
    lec_calculated = rmap calc_lec ["Email", "Punctaj curs"] lec
    exam_calculated = rmap calc_exam ["Nume", "Punctaj examen"] exam

    joined = tjoin "Nume"
               (tjoin "Email" 
                 (tjoin "Nume" email_map hw_calculated) 
                   lec_calculated)
                     exam_calculated
    
    grades_schema = ["Nume", "Punctaj Teme", "Punctaj Curs", "Punctaj Exam", "Punctaj Total"]
    final = rmap calc_final grades_schema joined
