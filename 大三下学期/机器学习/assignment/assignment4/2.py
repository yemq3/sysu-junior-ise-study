import numpy as np
from sklearn.svm import SVC

X = np.array([[1,2], [2,3], [3,3], [2,1], [3,2]])
Y = np.array([1, 1, 1, -1, -1])

clf = SVC()
clf.fit(X, Y)
print(clf.support_vectors_)