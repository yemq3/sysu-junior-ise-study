t = 0 # 初始点
h = 1 # 初始步长
a = 2 # 加步系数

f = lambda t: t ** 3 - 2 * t + 1

def search(f, t, h, a):
    k = 0
    temp = 0
    while True:
        if f(t+h) < f(t):
            temp = t
            t = t + h
            h = a * h
            k += 1
        else:
            if k == 0: # 反向探索
                h = -h
                temp = t+h
            else:
                start = min(temp, t+h)
                end = max(temp, t+h)
                return start, end

start, end = search(lambda x:2+x**2, 2, h, 0.1)
print(start, end)