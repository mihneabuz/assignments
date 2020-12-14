from time import time

#performance comparison of different implementations of fibbonaci series
#recursion vs tail recursion vs loop

def fib_recursion(n):
    if (n == 0): return 0
    if (n == 1): return 1
    return fib_recursion(n - 1) + fib_recursion(n - 2)

def fib_tailrecursion(n, a = 0, b = 1):
    if (n == 0): return b
    if (n == 1): return a + b
    return fib_tailrecursion(n - 1, b, a + b)

def fib_loop(n):
    if (n == 0): return 0
    if (n == 1): return 1
    a = 0;
    b = 1;
    for i in range(n - 1):
        a += b
        a, b = b, a
    return b


x = int(input())
begin_time = 0
duration = 0

print("Recursion:")
begin_time = time()
y1 = fib_recursion(x)
duration = time() - begin_time
print("time: {:.6f}s".format(duration))

print("Tail Recursion:")
begin_time = time()
y2 = fib_tailrecursion(x - 1)
duration = time() - begin_time
print("time: {:.6f}s".format(duration))

print("Loop:")
begin_time = time()
y3 = fib_loop(x)
duration = time() - begin_time
print("time: {:.6f}s".format(duration))

assert(y1 == y2 == y3)
