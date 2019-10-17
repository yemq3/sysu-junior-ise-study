#include "buffer.h"
#include <stdio.h>
#include <semaphore.h>
#define true 1

buffer_item buffer[BUFFER_SIZE];
sem_t empty;
sem_t full;
sem_t mutex;

int front = 0;
int rear = 0;

void init_buffer(){
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    sem_init(&mutex, 0, 1);
}

int insert_item(buffer_item item) {
    /* insert item into buffer
    return 0 if successful, otherwise
    return -1 indicating an error condition */

    sem_wait(&empty);
    sem_wait(&mutex);

    buffer[rear] = item;
    rear = (rear + 1) % BUFFER_SIZE;

    sem_post(&mutex);
    sem_post(&full);

    return 0;
}

int remove_item(buffer_item *item) {
    /* remove an object from buffer
    placing it in item
    return 0 if successful, otherwise
    return -1 indicating an error condition */

    sem_wait(&full); 
    sem_wait(&mutex);

    *item = buffer[front];
    front = (front + 1) % BUFFER_SIZE;

    sem_post(&mutex); 
    sem_post(&empty);
    
    return 0;
}



