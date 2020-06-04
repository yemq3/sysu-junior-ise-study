# 修正牛顿法

import numpy as np

epsilon = 0.01

def f(x):
    x1 = x[0][0]
    x2 = x[1][0]
    return 4 * (x1 + 1) ** 2 + 2 * (x2 - 1) ** 2 + x1 + x2 + 10

def g(x):
    grad = np.zeros((2,1))
    x1 = x[0][0]
    x2 = x[1][0]
    grad[0, 0] = 8 * x1 + 9
    grad[1, 0] = 4 * x2 - 3
    return grad

H = np.matrix([[8,0],
                [0,4]])

x = np.matrix([0,0]).T

print("初始值\n x={}\n\n g={}\n\n f={}".format(x, g(x), f(x)))
print("----------------------------------------------")

i = 1
while np.linalg.norm(g(x)) > epsilon:
    grad = g(x)
    G = H.I
    p = -G * grad
    step = -(grad.T * p) / (p.T * H * p)
    x = x + p * step
    print("第{}次迭代\n x={}\n\n g={}\n\n f={}".format(i, x, g(x), f(x)))
    print("----------------------------------------------")
    i += 1