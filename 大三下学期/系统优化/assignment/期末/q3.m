epsilon = 1e-3;

global a;

a = 5;

gamma = 100;

fun = @(x)x + gamma*p(x);

fminsearch(fun,rand)



function y = p(x)
    global a;
    y = max(a-x,0)^2;
end
