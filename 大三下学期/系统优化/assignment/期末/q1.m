% 单纯形法

epsilon = 0.1;

% 扩张系数
alpha = 1.1;

% 收缩系数
beta = 0.5;

x1 = [0;0];

x2 = [0;1];

x3 = [1;0];

i = 0;

x = [x1,x2,x3];

disp(x)
fprintf("----------------------------------------------\n")

while 1
    x = sortByf(x);
    old = f(x(:,1));
    middle_point = (x(:,1) + x(:,2)) / 2;
    reflect_point = middle_point + (middle_point - x(:,3));
    if f(reflect_point) < f(x(:,1))
        % 小于最优点，扩张
        extern_point = middle_point + alpha * (middle_point - x(:,3));
        if f(extern_point) < f(reflect_point)
            x(:,3) = extern_point;
        else
            x(:,3) = reflect_point;
        end
    elseif f(reflect_point) > f(x(:,1)) && f(reflect_point) < f(x(:,2))
        % 位于最优点和次优点之间，直接替代
        x(:,3) = reflect_point;
    elseif f(reflect_point) > f(x(:,2)) && f(reflect_point) < f(x(:,3))
        % 位于次优点和最差点之间，收缩
        shrink_point = middle_point + beta * (reflect_point - middle_point);
        x(:,3) = shrink_point;
    else
        % 比最差点还要差，压缩
        compress_point = middle_point - beta * (middle_point - x(:,3));
        if f(compress_point) < f(x(:,3))
            % 压缩点小于最差点
            x(:,3) = compress_point;
        else
            % 压缩点依旧大于最差点，压缩原三角形
            x(:,2) = (x(:,1) + x(:,2)) / 2;
            x(:,3) = (x(:,1) + x(:,3)) / 2;
        end
    end
    
    x = sortByf(x);
    disp(x)
    fprintf("----------------------------------------------\n");
    i = i + 1;
    % 避免除0，分母加一个很小的数
    if abs((f(x(:,3)) - f(x(:,1))) / (f(x(:,1) + 1e-5))) <= epsilon 
        break
    end
end

fprintf("迭代次数:%d\n",i);

        
function y = f(x)
    y=(x(1)-5)^2+(x(2)-6)^2;
end

% 将x按函数的值从小到大排序
function x = sortByf(x)
    y1 = f(x(:,1));
    y2 = f(x(:,2));
    y3 = f(x(:,3));
    value=[y1,y2,y3];
    [~, sortIdx] = sort(value);
    x = x(:,sortIdx);
end