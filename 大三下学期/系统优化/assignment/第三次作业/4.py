a = -3
b = 5
epsilon = 1e-3

f = lambda x: x * (x + 2)

while abs(a-b) > epsilon:
    print("搜索区间:[%f,%f]" % (a,b), end=" ")
    t1 = a + 0.618 * (b-a)
    t2 = a + 0.382 * (b-a)
    print("t1=%f t2=%f" % (t1, t2), end=" ")
    if f(t1) < f(t2):
        a = t2
        print("f(t1) < f(t2)")
    else:
        b = t1
        print("f(t1) > f(t2)")
print("答案:",(t1+t2)/2)