from multiprocessing import Process

class T():
    def __init__(self, val):
        self.val = val

def f(t):
    print(t.val)

if __name__ == '__main__':
    t = T(42)

    f(t)

    p = Process(target=f, args=(t,))
    p.start()