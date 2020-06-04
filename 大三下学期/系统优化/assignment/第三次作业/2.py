fx = lambda t: t * (t - 3)
gx = lambda t: 2 * t - 3

a = -3
b = 5
epsilon = 1e-1

while abs(a-b) > epsilon:
    print("搜索区间:[%d,%d]" % (a,b), end=" ")
    mid = (a+b) / 2
    print("(a+b)÷2 = %1f" % mid, end=" ")
    if gx(mid) < 0:
        a = mid
        print("2×t-3=%1f < 0" % gx(mid))
    elif gx(mid) > 0:
        b = mid
        print("2×t-3=%1f > 0" % gx(mid))
    else:
        print("答案:",mid)
        break

