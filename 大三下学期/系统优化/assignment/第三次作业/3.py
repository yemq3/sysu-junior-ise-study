f = lambda t: t ** 3 - 2 * t + 1
g = lambda t: 3 * t ** 2 - 2
gg = lambda t: 6 * t

a = 0
b = 3
epsilon = 1e-2

assert g(a) < 0
assert g(b) > 0

t = (a+b)/2
next_t = t - g(t)/gg(t)
while abs(t-next_t) >= epsilon:
    t = next_t
    next_t = t - g(t)/gg(t)
    print("t=%f" % next_t)
print(next_t, f(next_t))