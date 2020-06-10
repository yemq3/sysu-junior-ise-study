syms x1 x2;
epsilon = 1e-5;

f(x1,x2) = x1^2/4 + x2^2/2 - x1*x2 + x1 - x2;

g = gradient(f);

G = hessian(f);

% X = [0;0];
X = [1.5;1];

H = eye(2);

while norm(g(X(1),X(2))) > epsilon
    grad = g(X(1),X(2));
    p = -H * grad;
    alpha = -(grad' * p) / (p' * G * p);
    alpha = alpha(0,0);
    new_X = X + p * alpha;
    new_grad = g(new_X(1),new_X(2));
    s = new_X - X;
    y = new_grad - grad;
    H = H + (s * s') / (s' * y) - (H * y * y' * H) / (y' * H * y);
    X = new_X;
    fprintf("grad=[%.4f,%.4f]\n", g(X(1),X(2)));
    fprintf("f(x1,x2)=%.4f\n", f(X(1),X(2)));
    fprintf("X=[%.4f,%.4f]\n\n", X(1),X(2));
end