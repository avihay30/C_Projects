#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <string.h>

void* produce_A(void*);
void* produce_AA(void*);

sem_t s;

// Display work of two machines in 20 seconds
int main() {
    pthread_t thread1, thread2;
    sem_init(&s, 0, 0);
    int ans1, ans2;
    ans1 = pthread_create(&thread1, NULL, produce_A, (void*)"Thread A");
    ans2 = pthread_create(&thread2, NULL, produce_AA, (void*)"Thread AA");
    if (ans1 != 0 || ans2 != 0) {
        fprintf(stderr, "\nCan't create threads\n");
        return 0;
    }

    // main thread - limit work time to 20 seconds
    sleep(20);
    return 0;
}

// machine A - produce one item every 1 sec
void* produce_A(void* str) {
    while (1) {
        sleep(1);
        printf("Produced A\n");
        // notifying machine AA that new item has been produced
        sem_post(&s);
    }
}

// machine AA - produce one item 
// when there are 2 items of mechine A
void* produce_AA(void* str) {
    while (1) {
        // waiting for 2 items of mechine A
        sem_wait(&s);
        sem_wait(&s);
        printf("Collected AA\n");
    }
}