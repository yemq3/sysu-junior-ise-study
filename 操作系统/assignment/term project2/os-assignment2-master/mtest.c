// 打印代码段、数据段、BSS，栈、堆等的相关地址

//通过/proc/pid_number/maps，分析mtest各个内存段
//绘制图表，解释输出的每一段的各种属性，包括每一列的内容。
//为了让mtest程序驻留内存，可以在程序末尾加上长时睡眠，并将mtest在后台运行
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int a;
int b = 10;

int add(int a, int b){
    int sum = 0;
    sum = a+b;
    printf("sum in stack address : %p\n", &sum);
    return sum;
}

int main(){
    //BSS：未初始化的全局变量
    //数据段：已初始化的全局变量
    //代码段：存放程序执行代码
    //堆：动态分配的内存（malloc）
    //栈：局部变量
    printf("pid : %d\n", getpid());

    int *num = (int *)malloc(sizeof(int));
    *num = add(5, 10);
    printf("num in heap address : %p\n", num);
    printf("a in uninitialized data(bss) address : %p\n", &a);
    printf("b in initialized data address : %p\n", &b);
    printf("add function address : %p\n", &add);

    sleep(100);
}