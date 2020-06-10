syms x1 x2 t;
epsilon = 1e-4;
X = [-2;2];

f(x1,x2) = 100*(x2-x1^2)^2+(1-x1)^2;

g = gradient(f);

H = hessian(f);

i=1;
while norm(g(X(1),X(2))) > epsilon
    grad = g(X(1),X(2));
    G = inv(H(X(1),X(2)));
    p = -G * grad;
    t = armijo(f,g,X,p);
    X = X + t * p;
    i = i + 1;
    fprintf("grad=[%.4f,%.4f]\n", g(X(1),X(2)));
    fprintf("f(x1,x2)=%.4f\n", f(X(1),X(2)));
    fprintf("X=[%.4f,%.4f]\n\n", X(1),X(2));
end

function step = armijo(f,g,x,d)
    alpha = 0.2;
    beta = 0.5;
    max_iter = 50;
    m = 0;
    while m <= max_iter
        temp1 = x + beta^m*d;
        temp2 = x + alpha*beta^m.*g(x(1),x(2)).*d;
        if f(temp1(1), temp1(2)) <= f(temp2(1), temp2(2))
            best = m;
            break
        m=m+1;
        end 
    end
    step = beta^best;
end