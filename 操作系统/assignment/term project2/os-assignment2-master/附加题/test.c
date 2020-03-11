#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct a{
    int array[256];
};

int heap(int times){
    int i;
    struct a *p = malloc(times * sizeof(struct a));
    for(i = 0; i < times; i++){
        printf("%p\n", p[i].array);
    }
    free(p);
}

int stack(int times){
    int i;
    struct a p[times];
    for(i = 0; i < times; i++){
        printf("%p\n", p[i].array);
    }
}

int fact(int times){
    printf("%p\n", fact);
    if(times == 1)
        return 1;
    return times * fact(times - 1);
}

int main ()
{
    int counter = 0;
    int (*func[3])(int) = {heap, stack, fact};

    while(counter < 10000){
        int times = rand() % 100 + 1;
        int runfunc = rand() % 3;

        func[runfunc](times);
        counter += times;
    }

    return 0;
}