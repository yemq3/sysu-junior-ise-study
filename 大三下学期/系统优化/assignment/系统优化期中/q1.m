syms g(x) x;
epsilon = 1e-5;
X(1) = 1;

g(x) = (2*x-1)^2 + 4*(4-1024*x)^4;

grad = diff(g);

grad2 = diff(diff(g));

i = 1;
while norm(grad(X(i))) > epsilon
  X(i+1) = X(i) - grad(X(i))/grad2(X(i));
  i = i + 1;
  fprintf("grad=%.4f\n", grad(X(i)));
  fprintf("g(x)=%.4f\n\n", g(X(i)));
end