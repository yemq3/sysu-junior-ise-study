# 单纯形法

import numpy as np

def f(x):
    x1 = x[0][0]
    x2 = x[1][0]
    return x1 ** 2 + 2 * x2 ** 2 - 4 * x1 - 8 * x2 + 5

epsilon = 0.1

alpha = 1.1

beta = 0.5

x1 = np.matrix([0,0]).T

x2 = np.matrix([0.965,0.259]).T

x3 = np.matrix([0.259,0.965]).T

i = 1

x = [x1,x2,x3]

x.sort(key= lambda x: f(x))
print("初始值\n x={}\n\n f={}".format(x, f(x[0])))
print("----------------------------------------------")

while True:
    x.sort(key= lambda x: f(x))
    old = f(x[0])
    middle_point = (x[0] + x[1]) / 2
    reflect_point = middle_point + (middle_point - x[2])
    if f(reflect_point) < f(x[0]):
        # 小于最优点，扩张
        extern_point = middle_point + alpha * (middle_point - x[2])
        if f(extern_point) < f(reflect_point):
            x[2] = extern_point
        else:
            x[2] = reflect_point
    elif f(reflect_point) > f(x[0]) and f(reflect_point) < f(x[1]):
        # 位于最优点和次优点之间，直接替代
        x[2] = reflect_point
    elif f(reflect_point) > f(x[1]) and f(reflect_point) < f(x[2]):
        # 位于次优点和最差点之间，收缩
        shrink_point = middle_point + beta * (reflect_point - middle_point)
        x[2] = shrink_point
    else:
        # 比最差点还要差，压缩
        compress_point = middle_point - beta * (middle_point - x[2])
        if f(compress_point) < f(x[2]):
            # 压缩点小于最差点
            x[2] = compress_point
        else:
            # 压缩点依旧大于最差点，压缩原三角形
            x[1], x[2] = (x[0] + x[1]) / 2, (x[0] + x[2]) / 2
    
    x.sort(key= lambda x: f(x))
    print("第{}次迭代\n x={}\n\n f={}".format(i, x, f(x[0])))
    print("----------------------------------------------")
    i += 1
    if abs((f(x[2]) - f(x[0])) / f(x[0])) <= epsilon:
        break
