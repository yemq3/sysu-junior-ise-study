#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/syscall.h>
#define true 1
#define num_consumer 3
#define buffer_size 20
#define gettid() syscall(__NR_gettid)

int *buffer;
int front = 0;
sem_t *empty;
sem_t *full;
sem_t *mutex;

void init_buffer(){
    const int SIZE = sizeof(int) * buffer_size;
    const char *name = "buffer";
    int fd;

    fd = shm_open(name, O_RDONLY, 0666);
    buffer = (int *)mmap(0, SIZE, PROT_READ, MAP_SHARED, fd, 0);
    
    empty = sem_open("empty", O_RDWR);
    full = sem_open("full", O_RDWR);
    mutex = sem_open("mutex", O_RDWR);
}

int remove_item(int *item) {
    /* remove an object from buffer
    placing it in item
    return 0 if successful, otherwise
    return -1 indicating an error condition */

    sem_wait(full); 
    sem_wait(mutex);

    *item = buffer[front];
    front = (front + 1) % buffer_size;

    sem_post(mutex); 
    sem_post(empty);
    
    return 0;
}

int getSleepTime(double lambda)
{
	double pv = 0.0;
	pv = (double)rand() / RAND_MAX;
	while(pv == 0 || pv == 1)
	{
		pv = (double)rand() / RAND_MAX;
	}
	pv = -(1.0/lambda) * log(pv);
	return (int)pv;
}

void *consumer(void *param) {
    int item;
    double lambda = *(double *)param;
    while (true) { 
        /* sleep for a random period of time */
        sleep(getSleepTime(lambda));
        if (remove_item(&item))
            printf("report error condition");
        else
            printf("pid = %d tid = %ld consumer consumed %d \n",
                getpid(), gettid(), item);
    }
}

int main(int argc, char *argv[]) {
    /* 1. Get command line arguments argv[1] */
    double c = atof(argv[1]);
    if(c <= 0){
        printf("lambda must > 0\n");
        return 0;
    }

    /* 2. Initialize buffer */
    init_buffer();

    /* 3. Create consumer thread(s) */
    int i;
    pthread_t consumers[num_consumer];
    for(i = 0; i < num_consumer; i++)
        pthread_create(&consumers[i], NULL, consumer, &c);

    /* 4. Exit */
    for(i = 0; i < num_consumer; i++)
        pthread_join(consumers[i], NULL);

    return 0;
}
