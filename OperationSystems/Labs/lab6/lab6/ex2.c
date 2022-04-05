#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<string.h>
#include<time.h> // for gettimeofday()

void* incCounter(void*);
void* printCounters(void*);

long int* counters;

/* Program that runs 20 sec and create argv[1] threads
*  that incriment some element in counters,
   and prints the status of the counters every 2 sec */
int main(int argc, char* args[]) {
    int numOfThreads, i, ans;

    if (argc != 2) {
        fprintf(stderr, "No valid amount of arguments, should be only 2");
        return 1;
    }
    // mallocing args[1] + 1 threads
    numOfThreads = atoi(args[1]) + 1;
    pthread_t threads[numOfThreads];
    int tIndexes[numOfThreads];
    counters = (long int*)malloc(sizeof(long int) * (numOfThreads - 1));
    if (counters == NULL) {
        fprintf(stderr, "Allocation error");
        return 1;
    }

    /* creating args[1] threads for incConuter
       and anther one for printing counters */
    for (i = 0; i < numOfThreads; i++) {
        tIndexes[i] = i;
        // incrimenting all counters in the global array
        if (i < numOfThreads - 1) {
            ans = pthread_create(&threads[i], NULL, incCounter, (void*)&(tIndexes[i]));
        } else {
            ans = pthread_create(&threads[i], NULL, printCounters, (void*)&numOfThreads);
        }

        if (ans != 0) {
            fprintf(stderr, "\nCan't create threads\n");
            return 0;
        }
    }

    // waiting only for the print function - last thread
    // the function running only 20 seconds.
    pthread_join(threads[numOfThreads - 1], NULL);

    return 0;
}

// Incrimenting some counter in counters global array.
void* incCounter(void* cntIndex) {
    int countIdx = *(int*)cntIndex;
    while (1) counters[countIdx]++;
}

/* Printing counters global array only for 20 seconds.
   with interval of 2 seconds */
void* printCounters(void* numOfThreads) {
    int i, cntIndex, sizeOfArr;
    sizeOfArr = *(int*)numOfThreads - 1;

    // running only for 20 seconds
    for (i = 0; i < 10; i++) {
        sleep(2);
        printf("[");
        for (cntIndex = 0; cntIndex < sizeOfArr; cntIndex++) {
            if (cntIndex < sizeOfArr - 1) printf("%ld, ", counters[cntIndex]);
            else printf("%ld]\n", counters[cntIndex]);
        }
    }
}