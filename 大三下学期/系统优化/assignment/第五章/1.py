# 最速下降法

import numpy as np

epsilon = 0.01

f = lambda x: np.matrix([1,25]) * np.power(x, 2)

H = np.matrix([[2,0],
                [0,50]])

g = lambda x: np.multiply(np.matrix([2,50]).T, x)

x = np.matrix([2,2]).T

print("初始值\n x={}\n\n g={}\n\n f={}".format(x, g(x), f(x)))
print("----------------------------------------------")

i = 1
while np.linalg.norm(g(x)) > epsilon:
    grad = g(x)
    step = (grad.T * grad) / (grad.T * H * grad)
    x = x - grad * step
    print("第{}次迭代\n x={}\n\n g={}\n\n f={}".format(i, x, g(x), f(x)))
    print("----------------------------------------------")
    i += 1