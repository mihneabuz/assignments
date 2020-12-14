def insert_sort(l):
    for i in range(0, len(l)):
        j = i - 1;
        k = i;
        while(l[j] > l[k] and j >= 0):
            l[j], l[k] = l[k], l[j]
            j -= 1
            k -= 1
    return l

if __name__ == "__main__":
    print(insert_sort([3, 8, 5, 2, 9, 4]))
