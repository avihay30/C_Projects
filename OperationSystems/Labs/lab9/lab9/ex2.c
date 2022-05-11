#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>

#define M 3 // num of young
#define N 2 // num of old
void* oldManWantsToBuy(void*);
void* youngManWantsToBuy(void*);

int n = 0;
sem_t r, w;

// program that have young(reader)/old(writer) "man"
// that only one old man can buy at given same time
int main(int argc, char* argv[]) {
    if (argc != 1) {
        fprintf(stderr, "Invalid number of arguments");
        return 1;
    }

    sem_init(&r, 0, 1);
    sem_init(&w, 0, 1);

    pthread_t tYoungArr[M];
    pthread_t tOldArr[N];
    int i = 0, ans;
    int youngIdx[M], oldIdx[N];

    for (i = 0; i < M; i++) { // young buyers
        youngIdx[i] = i;
        ans = pthread_create(&tYoungArr[i], NULL, youngManWantsToBuy, (void*)&(youngIdx[i]));
        if (ans != 0) {
            fprintf(stderr, "\nCan't create threads\n");
            return 0;
        }
    }
    for (i = 0; i < N; i++) { // old buyer
        oldIdx[i] = i;
        ans = pthread_create(&tOldArr[i], NULL, oldManWantsToBuy, (void*)&(oldIdx[i]));
        if (ans != 0) {
            fprintf(stderr, "\nCan't create threads\n");
            return 0;
        }
    }
    // waiting for all threads
    for (i = 0; i < M; i++) {
        pthread_join(tYoungArr[i], NULL);
    }
    for (i = 0; i < N; i++) {
        pthread_join(tOldArr[i], NULL);
    }
    return 0;
}

// function for old man buying in shop
// like writer in read-write problem
void* oldManWantsToBuy(void* ind) { // writer
    int idx = *(int*)ind;
    printf("Old man #%d wants to buy\n", idx);
    // decreace num of old man - only one can get in
    sem_wait(&w);
    printf("Old man #%d is buying\n", idx);
    sleep(1);
    printf("Old man #%d is done\n", idx);
    // increace old man sem - 
    //let other young buyers/new old buyer work
    sem_post(&w);
}

// function for young man buying in shop
// like reader in read-write problem
void* youngManWantsToBuy(void* ind) { // reader
    int idx = *(int*)ind;
    printf("Young man #%d wants to buy\n", idx);
    sem_wait(&r);
    n++;
    // only if there is one young man
    if (n == 1) sem_wait(&w);
    sem_post(&r);
    printf("Young man #%d is buying\n", idx);
    sleep(1);
    sem_wait(&r);
    n--;
    printf("Young man #%d is done\n", idx);
    // if no young buyers left - let old buyers work
    if (n == 0) sem_post(&w);
    sem_post(&r);
}