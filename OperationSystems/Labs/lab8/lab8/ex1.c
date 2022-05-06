#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <string.h>

void* create_message(void*);

sem_t s1, s2;

// syncornizing Main theard print with thread-A print
int main() {
    pthread_t thread;
    // for enabling Thread A print, to run first
    sem_init(&s1, 0, 1);
    // for limiting main thread print to run after A
    sem_init(&s2, 0, 0);
    int i = 0, ans;
    ans = pthread_create(&thread, NULL, create_message, (void*)"Thread A");
    if (ans != 0) {
        fprintf(stderr, "\nCan't create threads\n");
        return 0;
    }

    for (i = 0; i < 10; i++) {
        // one first run, waiting for post in create_message function
        sem_wait(&s2);
        printf("The message #%d was printed. Thread B \n\n", i + 1);
        sem_post(&s1); // enabling Thread A to run
    }
    pthread_join(thread, NULL); // waiting for thread A

    return 0;
}

void* create_message(void* str) {
    int i = 0;
    for (i = 0; i < 10; i++) {
        sem_wait(&s1); // stopping until Main thread finished printing
        printf("I've wrote a message #%d. %s\n", i + 1, (char*)str);
        // notifying main Theard, that Thread A finished printing
        sem_post(&s2);
    }
}