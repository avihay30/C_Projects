#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<string.h>
#include<time.h> // for gettimeofday()

void* f(void*);

int curThread;

/* Program that prints the time slices
*  of argv[1] created threads.
*/
int main(int argc, char* args[]) {
    int numOfThreads, i, ans;

    if (argc != 2) {
        fprintf(stderr, "No valid amount of arguments, should be only 2");
        return 1;
    }
    
    numOfThreads = atoi(args[1]);
    pthread_t threads[numOfThreads];
    int tIndexes[numOfThreads];

    // creating all argv[1] threads.
    for (i = 0; i < numOfThreads; i++) {
        tIndexes[i] = i + 1;
        ans = pthread_create(&threads[i], NULL, f, (void*) &(tIndexes[i]));
        if (ans != 0) {
            fprintf(stderr, "\nCan't create threads\n");
            return 0;
        }
    }

    // waiting for all threads to finish.
    for (i = 0; i < numOfThreads; i++) {
        pthread_join(threads[1], NULL);
    }

    return 0;
}

void* f(void* tId) {
    struct timeval t1, t2;
    double elapsedTime;
    int threadId = *(int*)tId;

    while (1) {
        curThread = threadId;
        // start timer
        gettimeofday(&t1, NULL);

        // if threadId is running
        while (threadId == curThread) {
            // stop timer
            gettimeofday(&t2, NULL);
        }

        // compute and print the elapsed time in millisec
        elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
        elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
        printf("Time slice for thread %d = %lf  ms.\n", threadId, elapsedTime);
    }
}