from time import time
import lists as test
import mergeSort as sort



out = open("mergesort.out", "wt")
for i in test.lists:
    size = len(i)
    begin_time = time()
    sort.merge_sort(i)
    duration = time() - begin_time
    out.write("{} {}\n".format(size, duration))

out.close
