# 坐标轮换法

import numpy as np

epsilon = 0.1

def f(x):
    x1 = x[0][0]
    x2 = x[1][0]
    return x1 ** 2 + x2 ** 2 - x1 * x2 - 10 * x1 - 4 * x2 + 60

def g(x):
    grad = np.zeros((2,1))
    x1 = x[0][0]
    x2 = x[1][0]
    grad[0, 0] = 2 * x1 - x2 - 10
    grad[1, 0] = 2 * x2 - x1 - 4
    return np.matrix(grad)

G = np.matrix([[2,-1],
                [-1,2]])

x = np.matrix([0,0]).T

k = 1
H = np.matrix(np.eye(2))

print("初始值\n x={}\n\n g={}\n\n H={}\n\n f={}".format(x, g(x), H, f(x)))
print("----------------------------------------------")



while True:
    old_x = x
    for i in range(2):
        grad = g(x)
        p = H[i].T
        alpha = -(grad.T * grad) / (grad.T * G * p)
        x = x + p * alpha
    
    print("第{}次迭代\n x={}\n\n g={}\n\n f={}".format(k, x, g(x), f(x)))
    print("----------------------------------------------")
    k += 1
    if np.linalg.norm(x - old_x) < epsilon:
        break