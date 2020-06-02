/**
 * Implementation of thread pool.
 */

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include "threadpool.h"

#define QUEUE_SIZE 10
#define NUMBER_OF_THREADS 3

#define TRUE 1

// this represents work that has to be 
// completed by a thread in the pool
typedef struct 
{
    void (*function)(void *p);
    void *data;
}task;

task queue[QUEUE_SIZE];
int front = 0;
int rear = 0;
int size = 0;
int i;

// the work queue
task worktodo;

// the worker bee
pthread_t bee[NUMBER_OF_THREADS];

sem_t sem;
pthread_mutex_t mutex;
// insert a task into the queue
// returns 0 if successful or 1 otherwise, 
int enqueue(task t) 
{
    pthread_mutex_lock(&mutex);

    if(size >= QUEUE_SIZE)
        return 1;
    else{
        queue[rear] = t;
        rear = (rear + 1) % QUEUE_SIZE;
        size += 1;
    }
    sem_post(&sem);

    pthread_mutex_unlock(&mutex);

    return 0;
}

// remove a task from the queue
task dequeue() 
{
    pthread_mutex_lock(&mutex);

    worktodo = queue[front];
    front = (front + 1) % QUEUE_SIZE;
    size -= 1;

    pthread_mutex_unlock(&mutex);

    return worktodo;
}

// the worker thread in the thread pool
void *worker(void *param)
{
    while(1){
        sem_wait(&sem);
        // execute the task
        worktodo = dequeue(worktodo);
        execute(worktodo.function, worktodo.data);
    }
    // pthread_exit(0);
}

/**
 * Executes the task provided to the thread pool
 */
void execute(void (*somefunction)(void *p), void *p)
{
    (*somefunction)(p);
}

/**
 * Submits work to the pool.
 */
int pool_submit(void (*somefunction)(void *p), void *p)
{
    worktodo.function = somefunction;
    worktodo.data = p;

    enqueue(worktodo);

    return 0;
}

// initialize the thread pool
void pool_init(void)
{
    //initialize semaphores
    sem_init(&sem, 0, 0);

    //initialize mutual-exclusion locks
    pthread_mutex_init(&mutex,NULL);

    for(i = 0; i < NUMBER_OF_THREADS; i++)
        pthread_create(&bee[i],NULL,worker,NULL);
}

// shutdown the thread pool
void pool_shutdown(void)
{
    for(i = 0; i < NUMBER_OF_THREADS; i++)
        pthread_join(bee[i],NULL);
}
