#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

enum
{
    THINKING,
    HUNGRY,
    EATING
} state[5];
pthread_t philosophers[5];
pthread_mutex_t mutex;
pthread_cond_t self[5];
int identity[5] = {0, 1, 2, 3, 4};

void pickup_forks(int i);
void return_forks(int i);

void print_state(){
    int i;
    char str[10];
    printf("State:\n");
    for(i = 0; i < 5; i++){
        int s = state[i];
        switch(s){
            case 0:
                strcpy(str, "thinking");
                break;
            case 1:
                strcpy(str, "hungry");
                break;
            case 2:
                strcpy(str, "eating");
                break;
        }
        printf("Philospher %d is %s\n", i, str);
    }
    printf("\n");
}

void *brain(void *param)
{
    int id = *(int *)param;
    while (1)
    {
        int think_time = rand() % 100 + 1;
        sleep(think_time / 50);
        pickup_forks(id);
        // printf("Philospher %d is eating\n", id);

        int eat_time = rand() % 100 + 1;
        sleep(eat_time / 50);
        return_forks(id);
        // printf("Philospher %d is thinking\n", id);
    }
}


void init()
{
    // create five philosophers
    for (int i = 0; i < 5; i++)
    {
        pthread_create(&philosophers[i], NULL, brain, &identity[i]);
        state[i] = THINKING;
    }
    for (int i = 0; i < 5; i++)
        pthread_join(philosophers[i], NULL);

    // init the condition variables and mutex lock
    pthread_mutex_init(&mutex, NULL);
    for (int i = 0; i < 5; i++)
        pthread_cond_init(&self[i], NULL);
}

void test(int i)
{
    pthread_mutex_lock(&mutex);
    if ((state[(i + 4) % 5] != EATING) && (state[i] == HUNGRY) &&
        (state[(i + 1) % 5] != EATING))
    {
        state[i] = EATING;
        pthread_cond_signal(&self[i]); //通知在等待筷子的哲学家
    }
    pthread_mutex_unlock(&mutex);
}

void pickup_forks(int i)
{
    pthread_mutex_lock(&mutex);
    state[i] = HUNGRY;
    pthread_mutex_unlock(&mutex);

    test(i);

    pthread_mutex_lock(&mutex);
    // 避免在等筷子的时候进程中断导致打印被延后，从而使得同一个状态被打印两次
    if (state[i] != EATING){
        print_state();
        pthread_cond_wait(&self[i], &mutex); //拿不到筷子，等待
    }
    else{
        print_state();
    }
    pthread_mutex_unlock(&mutex);
}

void return_forks(int i)
{
    pthread_mutex_lock(&mutex);
    state[i] = THINKING;
    pthread_mutex_unlock(&mutex);

    // 查看左右两边需不需要筷子
    test((i + 4) % 5);
    test((i + 1) % 5);

    pthread_mutex_lock(&mutex);
    print_state();
    pthread_mutex_unlock(&mutex);
}

int main()
{
    init();
}
