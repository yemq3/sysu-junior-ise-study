#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "buffer.h"
#define true 1


void *producer(void *param) {
    buffer_item item;
    while (true) {
        /* sleep for a random period of time */
        sleep(rand() % 3 + 1);
        /* generate a random number */
        item = rand() % RAND_MAX;
        if (insert_item(item))
            printf("report error condition");
        else
            printf("producer produced %d \n", item);
    }
}


void *consumer(void *param) {
    buffer_item item;
    while (true) { /* sleep for a random period of time */
        sleep(rand() % 3 + 1);
        if (remove_item(&item))
            printf("report error condition");
        else
            printf("consumer consumed %d \n", item);
    }
}


int main(int argc, char *argv[]) {
    /* 1. Get command line arguments argv[1],argv[2],argv[3] */
    int sleep_time = atoi(argv[1]);
    int num_producer = atoi(argv[2]);
    int num_consumer = atoi(argv[3]);

    /* 2. Initialize buffer */
    init_buffer();

    /* 3. Create producer thread(s) */
    int i;
    pthread_t producers[num_producer];
    for(i = 0; i < num_producer; i++)
        pthread_create(&producers[i], NULL, producer, NULL);

    /* 4. Create consumer thread(s) */
    pthread_t consumers[num_consumer];
    for(i = 0; i < num_consumer; i++)
        pthread_create(&consumers[i], NULL, consumer, NULL);

    /* 5. Sleep */
    sleep(sleep_time);

    /* 6. Exit */
    for(i = 0; i < num_producer; i++)
        pthread_join(producers[i], NULL);

    for(i = 0; i < num_consumer; i++)
        pthread_join(consumers[i], NULL);

    return 0;
}


