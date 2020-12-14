from time import time
import lists as test
import insertionSort as sort


out = open("insertionsort.out", "wt")
for i in test.lists:
    size = len(i)
    begin_time = time()
    sort.insert_sort(i)
    duration = time() - begin_time
    out.write("{} {}\n".format(size, duration))

out.close
