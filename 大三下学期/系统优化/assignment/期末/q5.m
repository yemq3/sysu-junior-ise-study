N = 64; % 每次生成的数据个数
D_in = 1; % 输入维度
H_1 = 20; % 隐藏层1维度
H_2 = 30; % 隐藏层2维度
D_out = 1; % 输出层维度
learning_rate = 1e-3; % 学习率
iter = 10000; % 迭代次数

% 随机生成初始权重
w1 = rand([H_1, D_in]);
w2 = rand([H_2, H_1]);
w3 = rand([D_out, H_2]);

for t = 1:iter
    % 随机生成数据
    x = -3 + 6.*rand([D_in, N]);
    y = f(x);
    
    % 前向
    h1 = sigmoid(w1*x);
    
    h2 = sigmoid(w2*h1);

    y_pred = w3*h2;

    % 反向传播
    loss = sum(0.5.*(y_pred - y).^2) / N;
    
    if mod(t, 100) == 0
        fprintf("第%d次迭代，loss=%.4f\n",t,loss);
    end

    e = y_pred - y;
    
    delta3 = e; 
    delta2 = w3'*delta3.*h2.*(1-h2);
    delta1 = w2'*delta2.*h1.*(1-h1);
    
    grad_w3 = delta3*h2';
    grad_w2 = delta2*h1';
    grad_w1 = delta1*x';

    % 更新权重
    w1 = w1 - learning_rate * grad_w1;
    w2 = w2 - learning_rate * grad_w2;
    w3 = w3 - learning_rate * grad_w3;
end

% 画图对比
x = -3:0.1:3;
y = f(x);
plot(x,y)
hold on
h1 = sigmoid(w1*x);
h2 = sigmoid(w2*h1);
y_pred = w3*h2;
plot(x,y_pred)

function y = sigmoid(x)
    y = 1./(1+exp(-x));
end

 
function y = f(x)
%     y = 2.*x.^3+1.*x.^2+5;
    y = 2.*x.^2;
end