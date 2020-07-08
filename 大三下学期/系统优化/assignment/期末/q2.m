% 遗传算法
% 设精度为1e-4,则x,y的解空间被60000等分，需要16位二进制数进行编码

N = 20; % 种群大小
iter = 50; % 迭代次数
crossover_rate = 0.2; % 交配比例
mutation_rate = 0.05; % 变异概率
encode_l = 16; % 编码长度
parent_number = ceil(N*crossover_rate); % 繁殖个体数量
child_number = N - parent_number; % 替代个体数量
sum_p = cumsum(ones(parent_number,1)/parent_number); % 累加选择概率

fitness = ones(N, 1);

% 存储每一代的最优值
best_point = zeros(iter, 2);
best_fitness = zeros(iter, 1);

% 随机初始化
population = -3 + 6*rand(N,2);

for i = 1:iter
    fitness = calaFitness(population);
    
    % 大到小排序
    [fitness, index] = sort(fitness, 'descend');
    
    % 保存最优个体数据
    best_fitness(i) = fitness(1);
    best_point(i,:) = population(index(1), :);
    
    % 选择交配个体
    parent = population(index(1:parent_number), :);
    
    % 交叉繁殖
    for child = 1:2:child_number
        mother_index = ceil(rand * parent_number);
        father_index = ceil(rand * parent_number);
        % 避免自交
        while father_index == mother_index
            father_index = ceil(rand * parent_number);
        end
        mother = parent(mother_index, :);
        father = parent(father_index, :);
        
        bin_mother = encode(mother);
        bin_father = encode(father);
        
     
        % x,y分别更新
        child1_chrom = ["",""];
        child2_chrom = ["",""];
        for chrom = 1:2
            % 随机选个交叉点
            crossover_point = randi([2,14],1);
            
            mother_chrom = bin_mother(chrom, :);
            father_chrom = bin_father(chrom, :);
            
            mother_part1 = mother_chrom(1:crossover_point);
            mother_part2 = mother_chrom(crossover_point+1:encode_l);
            
            father_part1 = father_chrom(1:crossover_point);
            father_part2 = father_chrom(crossover_point+1:encode_l);
            
            child1_chrom(chrom) = [mother_part1, father_part2];
            child2_chrom(chrom) = [father_part1, mother_part2];
        end
        child1 = decode(child1_chrom);
        child2 = decode(child2_chrom);
        population(parent_number + child, :) = child1;
        population(parent_number + child + 1, :) = child2;
    end
    
    % 变异
    mutation_number = ceil(N * 2 * encode_l * mutation_rate);
    for j = 1:mutation_number
        % 随机选一个个体中的一个染色体的一个位置
        mutation_unit_index = randi([parent_number,N],1);
        mutation_chrom_index = randi([1,2],1);
        mutation_position = randi([1,16],1);
        % 翻转
        temp = encode(population(mutation_unit_index ,mutation_chrom_index));
        if temp(mutation_position) == "0"
            temp(mutation_position) = "1";
        else
            temp(mutation_position) = "0";
        end
        temp = decode(temp);
        population(mutation_unit_index ,mutation_chrom_index) = temp;
    end
end

plot(best_fitness)

[best_value, index] = max(best_fitness);

fprintf("最优点：[%.4f,%.4f]\n",best_point(index,:));
fprintf("函数值：%.4f\n",best_value);

% 二进制解码
function x = decode(x)
    x = -3+bin2dec(x)*6/(2^16-1);
end

% 编码到二进制
function x = encode(x)
    x = (x + 3) / (6 / 2^16);
    x = dec2bin(x, 16);
end

function fitness = calaFitness(population)
    N = size(population, 1);
    fitness = zeros(N,1);
    for i = 1:N
        fitness(i) = f(population(i,:));
    end
end

function y = f(x)
    y = 3 * (1 - x(1))^2 * exp(-x(1)^2 - (x(2) + 1)^2) - 10 * (x(1) / 5 - x(1)^3 - x(2)^5) * exp(-x(1)^2 - x(2)^2) - exp(-(x(1)+1)^2 - x(2)^2) / 3;
end