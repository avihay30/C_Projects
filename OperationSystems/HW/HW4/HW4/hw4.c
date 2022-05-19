#include<stdio.h>
#include <stdlib.h>
#include<pthread.h>
#include<semaphore.h>

#define ALLOC_ERR "AlloctionError: The program couldn't allocate memory!\n"
#define PACIENTS 12 // num of pacients (should be N+2)
#define WORKERS 3 // num of workers
#define SOFA_SEATS 4 // num of seats in sofa
#define N 10 // amount of space in clinc

typedef struct Node {
    struct Node* next;
    int value;
} Node;

typedef struct Que {
    Node* head, * tail;
    int size; // a current number of nodes
} Que, * PQue;


void* pacientWantsTreatment(void*);
void* dentalHygienistWorker(void*);
void Enqueue(PQue, int); //add a new member to list of the queue 
int Dequeue(PQue, int*); //delete member from the queue and "return" the deleted value using int

int pepoleToBeTreated = 0;
sem_t mutexInClinic, inClinic, inSofa, inTreatment, paymentMutex, doPayment, acceptedPayment;
sem_t freeWorker, workerInTreatment, finishTreatment;
sem_t sofaQMutex, standingQMutex;
Que sofaQ, standingQ;

// program that manege clinc of 3 Dental Hygienists
// the clinic has only space for N pepole.
// 3 pacients, 4 wait on sofa, all other stands
// after treatment has done pacient has to pay to some Dental Hygienists
int main(int argc, char* argv[]) {
    if (argc != 1) {
        fprintf(stderr, "Invalid number of arguments");
        return 1;
    }

    // init all Queues
    sofaQ.head = sofaQ.tail = standingQ.head = standingQ.tail = NULL;
    sofaQ.size = standingQ.size = 0;

    sem_init(&mutexInClinic, 0, 1); // mutex for accesing pepoleToBeTreated variable
    sem_init(&inClinic, 0, N); // sem that enable only N pepole in clinic
    sem_init(&inSofa, 0, SOFA_SEATS); // sem that enable only SOFA_SEATS pepole to seat on sofa
    sem_init(&inTreatment, 0, WORKERS); // sem that enable only WORKERS pepole to get treated
    sem_init(&paymentMutex, 0, 1); // mutex for accesing money-box (only one payment at a time)
    sem_init(&doPayment, 0, 0); // sem that enable payment - pacient post (++) for worker that waits (--)
    sem_init(&acceptedPayment, 0, 0); // sem that notify when payment is accepted (0-false, 1-true)
    sem_init(&freeWorker, 0, 0); // sem that notify pacients that some worker is free now
    sem_init(&workerInTreatment, 0, 0); // sem that notify sleeping worker to treat a pacient
    sem_init(&finishTreatment, 0, 0); // sem that notify pacients that treatment has been done
    sem_init(&sofaQMutex, 0, 1); // mutex for accesing sofaQMutex variable
    sem_init(&standingQMutex, 0, 1); // mutex for accesing standingQMutex variable

    // creating and running all threads
    pthread_t tPacientArr[PACIENTS];
    pthread_t tWorkerArr[WORKERS];
    int i = 0, ans;
    int pacientIdx[PACIENTS], workerIdx[WORKERS];

    for (i = 0; i < PACIENTS; i++) { // create threads for pacients
        pacientIdx[i] = i;
        ans = pthread_create(&tPacientArr[i], NULL, pacientWantsTreatment, (void*)&(pacientIdx[i]));
        if (ans != 0) {
            fprintf(stderr, "\nCan't create threads\n");
            return 0;
        }
    }
    for (i = 0; i < WORKERS; i++) { // create threads for workers
        workerIdx[i] = i;
        ans = pthread_create(&tWorkerArr[i], NULL, dentalHygienistWorker, (void*)&(workerIdx[i]));
        if (ans != 0) {
            fprintf(stderr, "\nCan't create threads\n");
            return 0;
        }
    }

    // waiting for all threads - will never finish
    // until Ctrl+C in terminal
    for (i = 0; i < PACIENTS; i++) {
        pthread_join(tPacientArr[i], NULL);
    }
    for (i = 0; i < WORKERS; i++) {
        pthread_join(tWorkerArr[i], NULL);
    }
    return 0;
}

// code for some pacients that wants to be treated
void* pacientWantsTreatment(void* ind) {
    int idx = *(int*)ind;
    int movedPecient = -1;
    while (1) {
        /* checking if there is space in clinc */
        sem_wait(&mutexInClinic);
        pepoleToBeTreated++;
        if (pepoleToBeTreated > N) { // if not enough space in clinic
            printf("I'm Pacient #%d, I'm out of clinic\n", idx);
        }
        sem_post(&mutexInClinic);

        sem_wait(&inClinic); // entering clinic if can, else wait outside

        sem_wait(&standingQMutex); // inserting pacient into standingQ
        printf("I'm Pacient #%d, I got into the clinic\n", idx);
        Enqueue(&standingQ, idx);
        sem_post(&standingQMutex); sleep(1);

        /// From Standing To Sofa ///
        // blocking non longest standing, until right pacient seat on sofa
        sem_wait(&inSofa); // seating on sofa if there is empty seat, else wait
        while (standingQ.head->value != idx) {
            sem_post(&inSofa);
            usleep(1);
            sem_wait(&inSofa);
        }

        sem_wait(&standingQMutex);
        Dequeue(&standingQ, &movedPecient); // pop pacient from standingQ
        printf("I'm Pacient #%d, I'm sitting on the sofa\n", idx); sleep(1);
        sem_wait(&sofaQMutex); // inserting pacient into sofaQ
        Enqueue(&sofaQ, idx);
        sem_post(&sofaQMutex);
        sem_post(&standingQMutex); // finished moving pacient standingQ -> sofaQ

        /// From Sofa To Treatment ///
        // blocking non longest seating on sofa, until right pacient gets treated
        sem_wait(&inTreatment); // going to get treated if possible, else wait
        while (sofaQ.head->value != idx) {
            sem_post(&inTreatment);
            usleep(1);
            sem_wait(&inTreatment);
        }
        sem_wait(&freeWorker); // waiting also to a free worker for getting treatment

        sem_wait(&sofaQMutex);
        printf("I'm Pacient #%d, I'm getting treatment\n", idx); sleep(1);
        sem_post(&inSofa); // out of sofa - going to treatment
        Dequeue(&sofaQ, &movedPecient); // pop pacient from sofaQ
        sem_post(&sofaQMutex); // finished pop from sofaQ

        /// In Treatment ///
        sem_post(&workerInTreatment); // notify some worker that treatment is needed
        sem_wait(&finishTreatment); // waiting for treatment to be finished
        sem_post(&inTreatment); // end of treatment - going to payment

        // payment - only one can be done at a time
        sem_wait(&paymentMutex);
        printf("I'm Pacient #%d, I'm paying now\n", idx); sleep(1);
        sem_post(&doPayment); // notifing worker that pacient is ready for payment
        sem_wait(&acceptedPayment); sleep(1);
        sem_post(&paymentMutex);

        sem_wait(&mutexInClinic); sleep(1);
        pepoleToBeTreated--; // pacient leaving clinic
        sem_post(&mutexInClinic);

        sem_post(&inClinic); sleep(1); // exiting clinic
    }
}

// code for Dental Hygienist
void* dentalHygienistWorker(void* ind) {
    int idx = *(int*)ind;
    while (1) {
        sem_post(&freeWorker); sleep(1); // notifing that this worker is free
        sem_wait(&workerInTreatment); // sleeping if no treatment is needed
        printf("I'm Dental Hygienist #%d, I'm working now\n", idx); sleep(1);
        sem_post(&finishTreatment);

        sem_wait(&doPayment); sleep(1); // waiting for payment to be done (post) by some pacient
        printf("I'm Dental Hygienist #%d, I'm getting a payment\n", idx);
        sem_post(&acceptedPayment); sleep(1); // recieved payment from pacient
    }
}

/* Function gets a queue and a number
   and inserts new node(with number) to queue(to tail). */
void Enqueue(PQue q, int new_elem)
{
    Node* temp;
    int delNum;
    temp = (Node*)malloc(sizeof(Node));
    if (temp == NULL) {
        printf(ALLOC_ERR);
        while (Dequeue(q, &delNum)) {} // free all queue.
    }
    // inserting temp(node) to tail. 
    temp->value = new_elem;
    temp->next = NULL;
    if (q->head == NULL)
        q->head = temp;
    else
        q->tail->next = temp;
    q->tail = temp;
    q->size++;
}

/* Function gets a queue and a dummy int pointer and deletes the head element in queue
   and assigning the removed element to the dummy int*/
int Dequeue(PQue q, int* del_value)
{
    Node* temp;
    if (q->head == NULL) { // if queue is empty.
        return 0; // failed
    }

    *del_value = q->head->value;
    // moving and deleting head.
    temp = q->head->next;
    free(q->head);
    q->head = temp;
    q->size--;
    return 1; // success
}