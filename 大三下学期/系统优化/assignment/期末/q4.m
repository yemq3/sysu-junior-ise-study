% 模拟退火

T = 1000; % 初始温度 
stop_T =0.01; % 停止温度
X = [0,0];
a = 2; % 邻域范围
rate = 1;
markov = 1000;

iter = 1;
best_X = X;
best_f = f(X,0);

while T > stop_T
    if T < 100
        rate = 0.1;
        markov = 10000;
    elseif T < 10
        rate = 0.01;
        markov = 100000;
    end
    for i = 1:markov
    % 随机扰动
        new_X = X + disturb([-a,a])*rate;
        
        delta = f(new_X,10000) - f(X,10000);

        if delta > 0
            % 函数值增大，接受新点
            X = new_X;
            if f(X,0) > best_f
                best_X = X;
                best_f = f(X,0);
            end
        else
            % 按一定概率接受
            if exp(delta/T) > rand
                X = new_X;
            end
        end
    end
    X = best_X;
    iter = iter + 1;
    T = T / log(iter);
end
disp(iter)
disp(best_X)
disp(best_f)

% 生成指定范围的随机扰动
function d = disturb(range)
    l = range(1);
    r = range(2);
    d = (l + (r-l) * [rand, rand]);
end

function y = f(x,gamma)
    y = 3 * (1 - x(1))^2 * exp(-x(1)^2 - (x(2) + 1)^2) - 10 * (x(1) / 5 - x(1)^3 - x(2)^5) * exp(-x(1)^2 - x(2)^2) - exp(-(x(1)+1)^2 - x(2)^2) / 3;
    % 加入罚函数
    y = y - gamma*max(3-x(1),0).^2 - gamma*max(x(2)-3,0).^2;
end