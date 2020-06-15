import numpy as np
import matplotlib.pyplot as plt

class SVM:
    def __init__(self, C, kernel="linear"):
        self.w = None
        self.b = None
        self.C = C
        self.e = []
        self.kernelType = kernel
        self.alpha = []
        self.Ecache = []

    def fit(self, x, y):   
        self.x = np.matrix(x)
        self.y = np.matrix(y).T
        self.m, self.n = np.shape(x)
        self.alpha = np.matrix(np.zeros((self.m,1))) # 拉格朗日乘子
        self.smo()
        self.w = self.computeW()
    
    def findBounds(self,i,j):
        if self.y[i] != self.y[j]:
            L = max(0,self.alpha[j]-self.alpha[i])
            H = min(self.C, self.C+self.alpha[j]-self.alpha[i])
        else:
            L = max(0,self.alpha[i]+self.alpha[j]-self.C)
            H = min(self.C,self.alpha[i]+self.alpha[j])
        return L,H

    def clipAlpha(self, alpha, L, H):
        if alpha > H:
            alpha = H
        elif alpha < L:
            alpha = L
        return alpha
    
    def kernel(self, i, j):
        if self.kernelType == "linear":
            return i * j.T

    def computeEi(self, i):
        fxi = np.multiply(self.alpha, self.y).T * self.kernel(self.x, self.x[i,:]) + self.b
        ei = fxi - self.y[i]
        return ei

    def computeW(self):
        m,n = np.shape(self.x)
        w = np.zeros((n,1))
        for i in range(m):
            w += np.multiply(self.alpha[i]*self.y[i], self.x[i,:].T)
        return w
    
    def selectJ(self, i):
        # 选择|Ei-Ej|最大的
        ei = self.computeEi(i)
        best_value = -1
        for j in range(self.m):
            if abs(ei-self.computeEi(j)) > best_value:
                best_index = j
                best_value = abs(ei-self.computeEi(j))
        return best_index
    
    def takeStep(self, i1, i2):
        eps = 1e-3
        if i1 == i2:
            return 0
        alpha1 = self.alpha[i1]
        y1 = self.y[i1]
        e1 = self.computeEi(i1)
        
        alpha2 = self.alpha[i2]
        y2 = self.y[i2]
        e2 = self.computeEi(i2)

        s = y1 * y2
        L, H = self.findBounds(i1, i2)
        if L == H:
            return 0
        k11 = self.kernel(self.x[i1,:],self.x[i1,:])
        k12 = self.kernel(self.x[i1,:],self.x[i2,:])
        k22 = self.kernel(self.x[i2,:],self.x[i2,:])
        eta = k11 + k22 - 2*k12
        if eta < 0:
            a2 = alpha2 + y2*(e2-e1)/eta
            a2 = self.clipAlpha(a2, L, H)
        else:
            c1 = eta/2
            c2 = y2*(e1-e2) - eta *alpha2
            Lobj = c1 * L * L + c2 * L
            Hobj = c1 * H * H + c2 * H
            if Lobj > Hobj + eps:
                a2 = L
            elif Lobj < Hobj - eps:
                a2 = H
            else:
                a2 = alpha2
        if abs(a2-alpha2) < eps*(a2+alpha2+eps):
            return 0
        a1 = alpha1+s*(alpha2-a2)
        if a1 < 0:
            a2 = s * a1
            a1 = 0
        elif a1 > self.C:
            t = a1-self.C
            a2 += s*t
            a1=C
        self.alpha[i1] = a1
        self.alpha[i2] = a2
        b1 = self.b - e1 - self.y[i1] * (self.alpha[i1] - alpha1) * k11 - self.y[i2] * (self.alpha[i2] - alpha2) * k22
        b2 = self.b - e2 - self.y[i1] * (self.alpha[i1] - alpha1) * k12 - self.y[i2] * (self.alpha[i2] - alpha2) * k22
        if 0 < self.alpha[i1] < self.C:
            self.b = b1
        elif 0 < self.alpha[i2] < self.C:
            self.b = b2
        else:
            self.b = (b1+b2)/2
        return 1
            

    def examineExample(self, i1):
        tol = 1e-3
        y1 = self.y[i1]
        alpha1 = self.alpha[i1]
        e1 = self.computeEi(i1)
        r1 = e1 * y1
        if (r1 < - tol and alpha1 < self.C) or (r1 > tol and alpha1 > 0):
            if len(self.alpha[self.alpha!=0]) + len(self.alpha[self.alpha!=self.C]) > 1:
                i2 = self.selectJ(i1)
                if self.takeStep(i1,i2):
                    return 1
            for i in range(self.m):
                if self.alpha[i] > 0 and self.alpha[i] < self.C:
                    i2 = i
                    if self.takeStep(i1,i2):
                        return 1
            for i in range(self.m):
                i2 = i
                if self.takeStep(i1,i2):
                    return 1
        return 0

    def smo(self):
        numChanged = 0
        examineAll = True
        self.b = 0
        while numChanged > 0 or examineAll:
            numChanged = 0
            if examineAll:
                for i in range(self.m):
                    numChanged += self.examineExample(i)
            else:
                for i in range(self.m):
                    if self.alpha[i] != 0 and self.alpha[i] != self.C:
                        numChanged += self.examineExample(i)
            if examineAll:
                examineAll = False
            elif numChanged == 0:
                examineAll = True

    def visualize(self):
        m, n = np.shape(self.x)
        for i in range(m):
            if self.y[i] == 1:
                plt.scatter(self.x[i,0], self.x[i,1], marker="+", c="r")
            else:
                plt.scatter(self.x[i,0], self.x[i,1], marker="_", c="b")

        # 分离超平面
        x1 = min(self.x[:,0])-1
        x2 = max(self.x[:,1])+1
        x1, x2 = x1[0,0], x2[0,0]
        a1, a2 = self.w
        a1, a2 = a1[0], a2[0]
        b = self.b
        y1, y2 = (-b-a1*x1)/a2,(-b-a1*x2)/a2
        y1, y2 = y1[0,0], y2[0,0]
        plt.plot([x1,x2], [y1,y2])

        # 间隔边界
        plt.plot([x1,x2], [y1+1/a2,y2+1/a2], '--')
        plt.plot([x1,x2], [y1-1/a2,y2-1/a2], '--')

        plt.xlabel('x')
        plt.ylabel('y')
        plt.show()


X = np.array([[1,2], [2,3], [3,3], [2,1], [3,2]])
Y = np.array([1, 1, 1, -1, -1])

# hisw = []
# hisb = []
# for _ in range(100):
#     svm = SVM(C=10)
#     svm.fit(X,Y)
#     hisb.append(svm.b)
# print(hisb)
# print(max(hisb) - min(hisb))

svm = SVM(C=20)
svm.fit(X,Y)
svm.visualize()
print(svm.w,svm.b,svm.alpha)