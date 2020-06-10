syms f(x) x;
l = 0.23;
f(x) = 8*exp(1-x) + 7*log(x);
grad = diff(f);

a = 1;
b = 2;

while abs(a-b) > l
    t1 = a + 0.618 * (b-a);
    t2 = a + 0.382 * (b-a);
    if f(t1) < f(t2)
        a = t2;
    else
        b = t1;
    end
    fprintf("搜索区间：[%.4f,%.4f]\n",a,b);
end