syms x1 x2;
epsilon = 1e-5;

f(x1,x2) = x1^2 + 2*x2^2 - 4*x1 - 2*x1*x2;

g = gradient(f);

H = hessian(f);

X = [1;1];

grad = g(X(1),X(2));
p = -grad;

while norm(g(X(1),X(2))) > epsilon
    grad = g(X(1),X(2));
    step = -(grad' * p) / (p' * H * p);
    step = step(0,0);
    X = X + step * p;
    beta = (norm(g(X(1),X(2))) / norm(grad))^2;
    p = -g(X(1),X(2)) + beta .* p;
    fprintf("grad=[%.4f,%.4f]\n", g(X(1),X(2)));
    fprintf("f(x1,x2)=%.4f\n", f(X(1),X(2)));
    fprintf("X=[%.4f,%.4f]\n\n", X(1),X(2));
end