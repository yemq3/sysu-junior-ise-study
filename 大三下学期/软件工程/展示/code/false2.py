from multiprocessing import Process

class T():
    def __init__(self, val):
        self.val = val

def f():
    print(t.val)

if __name__ == '__main__':
    t = T(42)

    f()

    p = Process(target=f, args=())
    p.start()