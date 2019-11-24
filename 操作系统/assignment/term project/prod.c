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
#define num_producer 3
#define buffer_size 20
#define gettid() syscall(__NR_gettid)

int *buffer;
int rear = 0;
sem_t *empty;
sem_t *full;
sem_t *mutex;

int insert_item(int item) {
    /* insert item into buffer
    return 0 if successful, otherwise
    return -1 indicating an error condition */

    sem_wait(empty);
    sem_wait(mutex);

    buffer[rear] = item;
    rear = (rear + 1) % buffer_size;

    sem_post(mutex);
    sem_post(full);

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

void *producer(void *param) {
    int item;
    double lambda = *(double *)param;
    while (true) {
        /* sleep for a random period of time */
        sleep(getSleepTime(lambda));
        /* generate a random number */
        item = rand() % RAND_MAX;
        if (insert_item(item))
            printf("report error condition");
        else
            printf("pid = %d tid = %ld producer produced %d \n", 
                getpid(), gettid(), item);
    }
}

void init_buffer(){
    const int SIZE = sizeof(int) * buffer_size;
    const char *name = "buffer";
    int fd;

    fd = shm_open(name,O_CREAT|O_RDWR,0666);
    ftruncate(fd, SIZE);
    buffer = (int *)mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    
    sem_unlink("empty");
    sem_unlink("full");
    sem_unlink("mutex");
    empty = sem_open("empty", O_CREAT|O_RDWR, 0666, buffer_size);
    full = sem_open("full", O_CREAT|O_RDWR, 0666, 0);
    mutex = sem_open("mutex", O_CREAT|O_RDWR, 0666, 1);
}


int main(int argc, char *argv[]) {
    /* 1. Get command line arguments argv[1] */
    double p = atof(argv[1]);
    if(p <= 0){
        printf("lambda must > 0\n");
        return 0;
    }
    


    /* 2. Initialize buffer */
    init_buffer();

    /* 3. Create producer thread(s) */
    int i;
    pthread_t producers[num_producer];
    for(i = 0; i < num_producer; i++)
        pthread_create(&producers[i], NULL, producer, &p);

    /* 4. Exit */
    for(i = 0; i < num_producer; i++)
        pthread_join(producers[i], NULL);

    return 0;
}
