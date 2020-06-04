# 共轭梯度法

import numpy as np

epsilon = 0.01

def f(x):
    x1 = x[0][0]
    x2 = x[1][0]
    return 2 * x1 ** 2 + x2 ** 2 - x1 * x2

def g(x):
    grad = np.zeros((2,1))
    x1 = x[0][0]
    x2 = x[1][0]
    grad[0, 0] = 4 * x1 - x2
    grad[1, 0] = 2 * x2 - x1
    return grad

H = np.matrix([[4,-1],
                [-1,2]])

x = np.matrix([1,1]).T

print("初始值\n x={}\n\n g={}\n\n f={}".format(x, g(x), f(x)))
print("----------------------------------------------")

i = 1
grad = g(x)
p = -grad
while np.linalg.norm(g(x)) > epsilon:
    grad = g(x)
    step = -(grad.T * p) / (p.T * H * p)
    x = x + step * p
    beta = (np.linalg.norm(g(x)) / np.linalg.norm(grad)) ** 2
    p = -g(x) + p * beta
    print("第{}次迭代\n x={}\n\n g={}\n\n f={}".format(i, x, g(x), f(x)))
    print("----------------------------------------------")
    i += 1