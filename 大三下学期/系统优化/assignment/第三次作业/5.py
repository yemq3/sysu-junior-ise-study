f = lambda x: 8 * x ** 3 - 2 * x ** 2 - 7 * x + 3
a = 0
b = 2
epsilon = 1e-3

def search(f, a, b, epsilon):
    h = (b - a) / 2
    s0 = a
    s1 = a + h
    s2 = b
    f0 = f(s0)
    f1 = f(s1)
    f2 = f(s2)
    h_hat = (3*f0 - 4*f1 + f2) * h / (2 * (f0 - 2*f1 + f2))
    s_hat = s0 + h_hat
    f_hat = f(s_hat)
    while abs(s2-s0) > epsilon:
        h = (s2 - s0) / 2
        h_hat = (3*f0 - 4*f1 + f2) * h / (2 * (f0 - 2*f1 + f2))
        print("插值点为:%f" % h_hat, end=" ")
        s_hat = s0 + h_hat
        f_hat = f(s_hat)
        if f1 < f_hat:
            if s1 < s_hat:
                s2 = s_hat
                f2 = f_hat
                s1 = (s2 + s0) / 2
                f1 = f(s1)
            else:
                s0 = s_hat
                f0 = f_hat
                s1 = (s2 + s0) / 2
                f1 = f(s1)
        else:
            if s1 > s_hat:
                s2 = s1
                s1 = s_hat
                f2 = f1
                f1 = f_hat
            else:
                s0 = s1
                s1 = s_hat
                f0 = f1
                f1 = f_hat
        print("搜索区间:[%f,%f]" % (s0,s2))
    return s1

res = search(f, a, b, epsilon)
print(res, f(res))