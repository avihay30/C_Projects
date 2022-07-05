#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define N 4

void* Func(void*);
void Calls(int, int);

sem_t sem_cities[N], mutex;

// Program to simulate calls between N cities
// cities 2,3 have high priorty above all other cities
int main(int argc, char* argv[]) {
    if (argc != 1) {
        fprintf(stderr, "Invalid number of arguments");
        return 1;
    }
    int i;
    srand(time(NULL));

    sem_init(&mutex, 0, 1);
    for (i = 0; i < N; i++) { // init all semaphores
        sem_init(&(sem_cities[i]), 0, 1);
    }

    pthread_t tArr[N];
    int indexes[N];

    for (i = 0; i < N; i++) { // creating all threads
        indexes[i] = i;
        if (pthread_create(&tArr[i], NULL, Func, &(indexes[i])) != 0) {
            fprintf(stderr, "\nCan't create threads\n");
            return 1;
        }
    }

    sleep(20); // allow threads to work only 20 seconds
    return 0;
}

// function for running threads
void* Func(void* arg) {
    int city1 = *(int*)arg;
    int city2, temp;

    while (1) {
        city2 = city1;
        while (city2 == city1) {
            city2 = rand() % N;
        }
        // left arg in Calls will be Min of the two
        if (city1 < city2)
            Calls(city1, city2); // city1 calls city2
        else
            Calls(city2, city1); // city2 calls city1    
    }
}

// function that preform a call between City1 <-> City2 
void Calls(int City1, int City2) {
    int i;
    sem_wait(&mutex);
    if (City1 + City2 != 5) { // if city 1,2 are not 2,3 -> allow multiple calls simultaneously
        sem_post(&mutex);
    }
    // stopping the two calling cities - unable to call them now
    sem_wait(&(sem_cities[City1]));
    sem_wait(&(sem_cities[City2]));

    printf("%d started to talk with %d\n", City1, City2);
    sleep(1);
    printf("%d finished to talk with %d\n", City1, City2);

    // releasing the two calling cities - free to call them now
    sem_post(&(sem_cities[City1]));
    sem_post(&(sem_cities[City2]));

    if (City1 + City2 == 5) { // if city 1,2 are indexes 3,2 -> allow other to make calls
        sem_post(&mutex);
    }
}