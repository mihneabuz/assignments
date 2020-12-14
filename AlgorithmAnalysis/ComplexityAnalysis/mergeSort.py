def merge(l1, l2):

    l = []
    i = 0
    j = 0
    while(i < len(l1) and j < len(l2)):
        if (l1[i] < l2[j]):
            l.append(l1[i])
            i += 1
        else:
            l.append(l2[j])
            j += 1

    for k in range(i, len(l1)):
        l.append(l1[k])
    for k in range(j, len(l2)):
        l.append(l2[k])

    return l
def merge_sort(l):
    if (len(l) == 1):
        return l

    n = int(len(l)/2)
    array1 = merge_sort(l[0 : n])
    array2 = merge_sort(l[n :])

    return merge(array1, array2)

if __name__ == "__main__":
    print(merge([2, 3, 5, 8], [1, 4, 7, 9]))
    print(merge_sort([347, 31, 444, 327, 704, 735, 62, 277, 146, 802]))
