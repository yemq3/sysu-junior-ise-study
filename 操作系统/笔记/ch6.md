# Synchronization Tools

## Background

- 程序有可能在任意位置被中断
- 并发执行时，对相同变量的访问结果是不可预测的

### Example

![1569197522116](ch6.assets/1569197522116.png)

count++（count--）的操作不是原子的，实际上的执行操作为

> register1 = count 
>
> register1 = register1 +1 
>
> count = register1

由于时序问题，多线程操作同一个全局变量`count`会导致数据的异常

![1569197778024](ch6.assets/1569197778024.png)

## The Critcal-Section Problem

Critcal-Section（临界区）：多个进程共享数据，每个进程对共享数据的访问，更新的地方称为临界区。

### Three requirements

- 互斥(mutual exclusion)
  - 只有一个进程进入临界区
- 空闲让进(progress)
  - 临界区空闲时，只有需要进行临界区操作的进程可以进入临界区
- 有限等待(bounded wait)

### Solutions

- raise a flag
- maintain a global flag(turn)
- turn+flag

## Peterson's Solution

进程共享两个变量`turn`和`flag`

`turn`决定哪个进程进入临界区，`flag`代表每个进程是否需要进入临界区

```
int turn; 
boolean flag[2];
while (true) { 
    flag[i] = true; 
    turn = j; 
    while (flag[j] && turn == j) 
        ;
        /* critical section */
    flag[i] = false;
    	/*remainder section */
}
```

- mutual exclusion
  - 两个进程进入临界区时，flag和turn变量的值不同，不存在两个进程同时进入临界区的情况
- progress
  - 一个进程退出临界区时，flag会被改变，保证了空闲让进
- bounded wait
  - 同上

但在现代操作系统上这一方法并不一定正确，因为不能保证操作都是顺序执行的，下图进程2中的`flag=true`的执行时间可能早于`x=100`，从而导致进程1输出的为0

![1569199472850](ch6.assets/1569199472850.png)

![1569200287840](ch6.assets/1569200287840.png)

## Hardware Support for Synchronization

### Memory Barriers

- Thread1

```c
while (!flag) 
	memory_barrier(); 
print x;
```

- Thread2

```c
x = 100; 
memory_barrier(); 
flag = true;
```

### Hardware Instructions

保证为原子操作

两种方法：

- `test_and_set()`

```C
boolean test_and_set(boolean *target) { 
    boolean rv = *target; 
    *target = true;
    return rv;
}
```

```c
do {
	while (test_and_set(&lock)) 
        ; /* do nothing */
		/* critical section */
	lock = false;
		/* remainder section */ 
} while (true);

```

- `compare_and_swap() `

```c
int compare_and_swap(int *value, int expected, int new value) { 
    int temp = *value;
	if (*value == expected) 
        *value = new value;
	return temp;
}
```

```c
while (true) { 
    while (compare_and_swap(&lock, 0, 1) != 0) 
        ; /* do nothing */
		/* critical section */
	lock = 0;
		/* remainder section */
}
```

### Atomic Variables

通过硬件实现原子操作，保证在原子操作时不会被打断

```c
void increment(atomic int *v) { 
    int temp; 
    do {
        temp = *v; 
    } while (temp != compare and swap(v, temp, temp+1)); 
}
```

## Mutex Locks

???

## Semaphores

信号量只能是0或1

两个原子操作

- `wait()`

```c
wait(S) { 
    while (S <= 0) ; // busy wait 
    S--; 
} 
```

- `signal()`

```c
signal(S) { 
	S++;
}
```

### Usage

```
P1:
	S1;
	signal(synch);
P2：
	wait(synch)
	S2;
```

### Semaphore Implementation

```c
typedef struct{
    int value;
    struct process *list;
}semaphore;

wait(semaphore *S) { 
    S->value--; 
    if (S->value < 0) {
        add this process to S->list;
        sleep();
    } 
}

signal(semaphore *S) {
    S->value++;
    if (S->value <= 0) { 
        remove a process P from S->list;
        wakeup(P);
    } 
}
```

缺点：不够安全，使用不当会产生死锁

## Monitors

编译器添加

![img](ch6.assets/200px-Monitor_(synchronization)-SU.png)

## Liveness

### Deadlock

###  Priority Inversion

## Evalution

6.13

6.33