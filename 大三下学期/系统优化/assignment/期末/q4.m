% 模拟退火

T = 1000; % 初始温度 
stop_T =1; % 停止温度
X = [0,0];
a = 10; % 邻域范围
markov = 10;

mean_value = zeros(100,1);
std_value = zeros(100,1);

for a = 0.1:0.1:10
    val = zeros(100,1);
    % 运行100次求平均值和标准差
    for time = 1:100
        [best_value, best_point] = SA(T,stop_T,a,markov,X,@f);
        val(time)=best_value;
    end
    mean_value(int8(a*10)) = mean(val);
    std_value(int8(a*10)) = std(val);
end
% fprintf("最优点：[%.4f,%.4f]\n",best_point);
% fprintf("函数值：%.4f\n",best_value);
plot(0.1:0.1:10,mean_value,0.1:0.1:10,std_value);
legend("均值","标准差");
xlabel('alpha')
% fprintf("100次运算结果均值:%.4f，标准差:%.4f\n",mean(val),std(val));


function d = disturb(range)
    l = range(1);
    r = range(2);
    d = (l + (r-l) * [rand, rand]);
end

function y = f(x,gamma)
    y = 3 * (1 - x(1))^2 * exp(-x(1)^2 - (x(2) + 1)^2) - 10 * (x(1) / 5 - x(1)^3 - x(2)^5) * exp(-x(1)^2 - x(2)^2) - exp(-(x(1)+1)^2 - x(2)^2) / 3;
    % 加入罚函数限制可行域
    y = y - gamma*max(-x(1)-3,0).^2 - gamma*max(-3+x(2),0).^2;
end

function [best_value, best_point] = SA(start_T,stop_T,a,markov,start_point,f)
    iter = 1;
    T = start_T;
    X = start_point;
    best_point = X;
    best_value = f(X,0);
    while T > stop_T
        for i = 1:markov
            % 随机扰动
            new_X = X + disturb([-a,a]);

            delta = f(new_X,10000) - f(X,10000);
            if delta > 0
                % 函数值增大，接受新点
                X = new_X;
                % 保存最佳点
                if f(X,0) > best_value
                    best_point = X;
                    best_value = f(X,0);
                end
            else
                % 函数值减小，按一定概率接受
                if exp(delta/T) > rand
                    X = new_X;
                end
            end
        end
        % 让X回到最优点，否则X很有可能跑到很远
        X = best_point;
        iter = iter + 1;
        T = T / log(iter);
    end

end
