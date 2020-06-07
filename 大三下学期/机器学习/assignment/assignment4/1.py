# 试求最大间隔分离超平面和分类决策函数，并在图上画出分离超平面，间隔边界以及支持向量。 

import numpy as np
import matplotlib.pyplot as plt

class SVM:
    def __init__(self):
        self.w = 1
        self.b = 0
        self.support_vector = []

    def fit(self, x, y):   
        self.x = x
        self.y = y
        pass

    def predict(self, x):
        return np.sign(np.dot(self.w, np.array(x)) + self.b)

    def visualize(self):
        for x, y in zip(self.x, self.y):
            if y == 1:
                plt.scatter(x[0], x[1], marker="+", c="r")
            else:
                plt.scatter(x[0], x[1], marker="_", c="b")

        x = np.linspace(1,3)
        y = 1 * x + 0
        plt.plot(x, y)

        plt.xlabel('x')
        plt.ylabel('y')
        plt.show()


    def _geometric_margin(self, x):
        return (np.dot(self.w, np.array(x)) + self.b) / np.linalg.norm(self.w)



# positive = np.array([[1,2], [2,3], [3,3]])
# negative = np.array([[2,1], [3,2]])

# plt.scatter(positive[:,0], positive[:,1], marker="+")
# plt.scatter(negative[:,0], negative[:,1], marker="_")

# x = np.linspace(1,3)
# y = 1 * x + 0
# plt.plot(x, y)
# plt.xlabel('x')
# plt.ylabel('y')
# plt.show()

X = np.array([[1,2], [2,3], [3,3], [2,1], [3,2]])
Y = np.array([1, 1, 1, 0, 0])

svm = SVM()
svm.fit(X,Y)
svm.visualize()