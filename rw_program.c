#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_READERS 3
#define NUM_WRITERS 2

sem_t rw_mutex;
sem_t mutex;
int readers_count = 0;
int shared_data = 0;

void *reader(void *arg) {
    int id = *(int*)arg;

    sem_wait(&mutex);
    readers_count++;
    if (readers_count == 1)
        sem_wait(&rw_mutex);
    sem_post(&mutex);

    printf("Reader %d is READING: shared_data = %d\n", id, shared_data);
    sleep(1);

    sem_wait(&mutex);
    readers_count--;
    if (readers_count == 0)
        sem_post(&rw_mutex);
    sem_post(&mutex);

    return NULL;
}

void *writer(void *arg) {
    int id = *(int*)arg;

    sem_wait(&rw_mutex);

    shared_data++;
    printf("Writer %d is WRITING: shared_data = %d\n", id, shared_data);
    sleep(1);

    sem_post(&rw_mutex);

    return NULL;
}

int main() {
    pthread_t readers[NUM_READERS], writers[NUM_WRITERS];
    int reader_ids[NUM_READERS], writer_ids[NUM_WRITERS];

    sem_init(&rw_mutex, 0, 1);
    sem_init(&mutex, 0, 1);

    for (int i = 0; i < NUM_WRITERS; i++) {
        writer_ids[i] = i + 1;
        pthread_create(&writers[i], NULL, writer, &writer_ids[i]);
    }

    for (int i = 0; i < NUM_READERS; i++) {
        reader_ids[i] = i + 1;
        pthread_create(&readers[i], NULL, reader, &reader_ids[i]);
    }

    for (int i = 0; i < NUM_WRITERS; i++)
        pthread_join(writers[i], NULL);
    for (int i = 0; i < NUM_READERS; i++)
        pthread_join(readers[i], NULL);

    sem_destroy(&rw_mutex);
    sem_destroy(&mutex);

    printf("All done. Final shared_data = %d\n", shared_data);
    return 0;
}
