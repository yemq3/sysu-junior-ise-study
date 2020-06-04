# 牛顿法

import numpy as np

epsilon = 0.01

def f(x):
    x1 = x[0][0]
    x2 = x[1][0]
    return 60 - 10 * x1 - 4 * x2 + x1 ** 2 + x2 ** 2 - x1 * x2

def g(x):
    grad = np.zeros((2,1))
    x1 = x[0][0]
    x2 = x[1][0]
    grad[0, 0] = -10 + 2 * x1 - x2
    grad[1, 0] = -4 + 2 * x2 - x1
    return grad

H = np.matrix([[2,-1],
                [-1,2]])

x = np.matrix([0,0]).T

print("初始值\n x={}\n\n g={}\n\n f={}".format(x, g(x), f(x)))
print("----------------------------------------------")

i = 1
while np.linalg.norm(g(x)) > epsilon:
    grad = g(x)
    x = x - H.I * grad
    print("第{}次迭代\n x={}\n\n g={}\n\n f={}".format(i, x, g(x), f(x)))
    print("----------------------------------------------")
    i += 1