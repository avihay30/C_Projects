#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>

#define K 5
#define N 10
void* tFunc(void*);
void stkPush(int num);
int stkPop();

int stk[N], idx = 0;
sem_t mutex, empty, full;

// Program that has K threads that 
// call Push/Pop on global stack
int main(int argc, char* argv[]) {
    if (argc != 1) {
        fprintf(stderr, "Invalid number of arguments");
        return 1;
    }

    sem_init(&mutex, 0, 1);
    sem_init(&empty, 0, N);
    sem_init(&full, 0, 0);

    pthread_t tArr[K];
    int i = 0, ans[K];

    for (i = 0; i < K; i++)
        ans[i] = pthread_create(&tArr[i], NULL, tFunc, NULL);

    for (i = 0; i < K; i++)
        if (ans[i] != 0) {
            fprintf(stderr, "\nCan't create threads\n");
            return 0;
        }

    sleep(10);
    return 0;
}

void* tFunc(void* p) {
    int num;
    while (1) {
        num = rand() % 100;
        if (rand() % 2) {
            stkPush(num);
            printf("Push %d\n", num);
        }
        else {
            printf("Pop %d\n", stkPop());
        }
        sleep(1);
    }
}

void stkPush(int num) { // producer
    sem_wait(&empty); // decrease empty by 1
    sem_wait(&mutex); // start - Critical-section
    stk[idx++] = num; // push to stack
    sem_post(&mutex); // end - Critical-section
    sem_post(&full);
}

int stkPop() { // consumer
    int removed;
    sem_wait(&full); // stops if stack is empty
    sem_wait(&mutex); // start - Critical-section
    removed = stk[--idx]; // pop stack and save in local variable
    sem_post(&mutex); // end - Critical-section
    sem_post(&empty); // increase empty by 1

    return removed;
}
