#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>

int main(){
    syscall(333);
    return 0;
}