#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

double average = 0;
int minimum = INT_MAX;
int maximun = INT_MIN;

void *getAverage(void *param);
void *getMinimum(void *param);
void *getMaximun(void *param);

struct param{
    int num;
    char** data;
};

int main(int argc, char *argv[]){
    pthread_t tid[3];
    pthread_attr_t attr;
    struct param p;
    p.num = argc;
    p.data = argv;

    pthread_attr_init(&attr);
    pthread_create(&tid[0], &attr, getAverage, &p);
    pthread_create(&tid[1], &attr, getMinimum, &p);
    pthread_create(&tid[2], &attr, getMaximun, &p);
    for(int i = 0; i < 3; i++)
        pthread_join(tid[i], NULL);

    printf("The average value is %f\n",average);
    printf("The minimun value is %d\n",minimum);
    printf("The maximun value is %d\n",maximun);

    return 0;
}

void *getAverage(void *param){
    struct param *p = (struct param*)param;
    int i;
    for(i=1; i < p->num; i++){
        average += atoi(p->data[i]);
    }
    average /= (p->num - 1);
    pthread_exit(0);
}

void *getMinimum(void *param){
    struct param *p = (struct param*)param;
    int i;
    for(i=1; i < p->num; i++){
        if(atoi(p->data[i]) < minimum){
            minimum = atoi(p->data[i]);
        }
    }
    pthread_exit(0);
}

void *getMaximun(void *param){
    struct param *p = (struct param*)param;
    int i;
    for(i=1; i < p->num; i++){
        if(atoi(p->data[i]) > maximun){
            maximun = atoi(p->data[i]);
        }
    }
    pthread_exit(0);
}