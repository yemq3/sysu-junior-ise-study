# DFP

import numpy as np

epsilon = 0.01

def f(x):
    x1 = x[0][0]
    x2 = x[1][0]
    return 4 * (x1 - 5) ** 2 + (x2 - 6) ** 2

def g(x):
    grad = np.zeros((2,1))
    x1 = x[0][0]
    x2 = x[1][0]
    grad[0, 0] = 8 * x1 - 40
    grad[1, 0] = 2 * x2 - 12
    return grad

G = np.matrix([[8,0],
                [0,2]])

x = np.matrix([8,9]).T

i = 1
H = np.matrix(np.eye(2))

print("初始值\n x={}\n\n g={}\n\n H={}\n\n f={}".format(x, g(x), H, f(x)))
print("----------------------------------------------")


while np.linalg.norm(g(x)) > epsilon:
    grad = g(x)
    p = -H * grad
    alpha = -(grad.T * p) / (p.T * G * p)
    new_x = x + p * alpha
    new_grad = g(new_x)
    s = new_x - x
    y = new_grad - grad
    H = H + (s * s.T) / (s.T * y) - (H * y * y.T *H) / (y.T * H * y)
    x = new_x
    print("第{}次迭代\n x={}\n\n g={}\n\n H={}\n\n f={}".format(i, x, g(x), H, f(x)))
    print("----------------------------------------------")
    i += 1