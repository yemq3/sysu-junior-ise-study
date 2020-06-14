import numpy as np
import matplotlib.pyplot as plt

class SVM:
    def __init__(self, C, kernel="linear"):
        self.w = None
        self.b = None
        self.C = C
        self.e = []
        self.kernel = kernel
        self.alpha = []

    def fit(self, x, y):   
        self.x = np.matrix(x)
        self.y = np.matrix(y).T
        self.m, self.n = np.shape(x)
        self.alpha = np.matrix(np.zeros((m,1))) # 拉格朗日乘子
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
        if self.kernel = "linear":
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
            if abs(ei-self.computeEi(j,self.b)) > best_value:
                best_index = j
                best_value = abs(ei-self.computeEi(j,self.b))
        return best_index
    
    def takeStep(self, i1, i2):
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
        if eta > 0:
            a2 = alpha2 + y2*(e1-e2)/eta
            a2 = self.clipAlpha(a2, L, H)
        else:
            
            

    def examineExample(self, i2):
        tol = 1e-3
        y2 = self.y[i2]
        alpha2 = self.alpha[i2]
        e2 = self.computeEi(i2)
        r2 = e2 * y2
        if (r2 < - tol and alpha2 < self.C) or (r2 > tol and alpha2 > 0):
            if len(self.alpha[self.alpha!=0]) + len(self.alpha[self.alpha!=C]) > 1:
                i1 = selectJ(i2)
                if self.takeStep(i1,i2):
                    return 1
            for i in range(self.m):
                if self.alpha[i] != 0 and self.alpha[i] != C:
                    i1 = i
                    if self.takeStep(i1,i2):
                        return 1
            for i in range(self.m):
                i1 = i
                if self.takeStep(i1,i2):
                    return 1
        return 0

    def smo(self):
        numChanged = 0
        examineAll = True
        while numChanged > 0 or examineAll:
            numChanged = 0
            if examineAll:
                for i in range(self.m):
                    numChanged += self.examineExample(i)
            else:
                for i in range(self.m):
                    if self.alpha[i] != 0 and self.alpha[i] != C:
                        numChanged += self.examineExample(i)
            if examineAll:
                examineAll = False
            elif numChanged == 0:
                examineAll = True
