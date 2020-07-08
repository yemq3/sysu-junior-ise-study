epsilon = 1e-3;

global a;

a = 5;

gamma = 10000;

fun = @(x)x + gamma*p(x);

val = fminsearch(fun,rand);

fprintf("最优解:%.4f\n",val)

% 罚函数
function y = p(x)
    global a;
    y = max(a-x,0)^2;
end
