#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>

#define NUM_OF_JUDGES 5
#define NUM_OF_JURY 15
#define POSSIBLE_JURY_IN_COURT 10
#define POSSIBLE_JUDGES_IN_COURT 1

void* juryFunc(void*); // jury thread function
void* judgeFunc(void*); // judges thread function
void enterJury(int idx);
void exitJury();
void enterJudge(int idx);
void exitJudge();
void startJudgmentIfPossible();
void exitFromCourtIfImLast();

int juryAmountIn = 0; // number of juries that entered the court
int judgesAmountIn = 0; // number of judges that entered the court
int isJudgmentDone = 0; // flag variable the indicates whether Judgment is done or not
sem_t mutex, semJudges, semJury;

int main(int argc, char* argv[]) {
    if (argc != 1) {
        fprintf(stderr, "Invalid number of arguments");
        return 1;
    }

    sem_init(&mutex, 0, 1); // for accessing mutual variables
    sem_init(&semJudges, 0, POSSIBLE_JUDGES_IN_COURT);
    sem_init(&semJury, 0, POSSIBLE_JURY_IN_COURT);

    pthread_t tJudges[NUM_OF_JUDGES];
    pthread_t tJury[NUM_OF_JURY];
    int judgesIdx[NUM_OF_JUDGES], juryIdx[NUM_OF_JURY];
    int i, ans;

    for (i = 0; i < NUM_OF_JUDGES; i++) { // creating all judges
        judgesIdx[i] = i;
        ans = pthread_create(&tJudges[i], NULL, judgeFunc, (void*)&(judgesIdx[i]));

        if (ans != 0) {
            fprintf(stderr, "\nCan't create threads\n");
            return 0;
        }
    }

    for (i = 0; i < NUM_OF_JURY; i++) { // creating all juries
        juryIdx[i] = i;
        ans = pthread_create(&tJury[i], NULL, juryFunc, (void*)&(juryIdx[i]));

        if (ans != 0) {
            fprintf(stderr, "\nCan't create threads\n");
            return 0;
        }
    }

    sleep(20); // allow running all thread for only 20 sec
    return 0;
}

// handles all the entring and exiting court of jury
void* juryFunc(void* arg) {
    int idx = *(int*)arg;

    while (1) {
        enterJury(idx);
        exitJury();
    }

}

// handles all the entring and exiting court of judges
void* judgeFunc(void* arg) {
    int idx = *(int*)arg;

    while (1) {
        enterJudge(idx);
        exitJudge();
    }
}

void enterJury(int idx) {
    // Trying to enter court
    sem_wait(&semJury);

    // In court
    sem_wait(&mutex);
    juryAmountIn++;
    printf("JURY %d IN COURT\n", idx);
    startJudgmentIfPossible();
    sem_post(&mutex);
}

void exitJury() {
    // waiting to judgment to be done
    while (isJudgmentDone == 0);

    // exiting court
    sem_wait(&mutex);
    juryAmountIn--;
    exitFromCourtIfImLast();
    sem_post(&mutex);
}

void enterJudge(int idx) {
    // Trying to enter court
    sem_wait(&semJudges);

    // In court
    sem_wait(&mutex);
    judgesAmountIn++;
    printf("JUDGE %d IN COURT\n", idx);
    startJudgmentIfPossible();
    sem_post(&mutex);
}

void exitJudge() {
    // waiting to judgment to be done
    while (isJudgmentDone == 0);

    // exiting court
    sem_wait(&mutex);
    judgesAmountIn--;
    exitFromCourtIfImLast();
    sem_post(&mutex);
}

// last thread that goes into the court starts the judgment
void startJudgmentIfPossible() {
    if (juryAmountIn == POSSIBLE_JURY_IN_COURT &&
        judgesAmountIn == POSSIBLE_JUDGES_IN_COURT) {
        printf("START\n");
        sleep(2);
        printf("END\n");
        isJudgmentDone = 1;
    }
}

// last thread that goes out of the court
// posting all semaphore to allow other jury/judges to enter the court
void exitFromCourtIfImLast() {
    if (juryAmountIn == 0 && judgesAmountIn == 0) {
        // Reseting flags
        isJudgmentDone = 0;
        int i;

        for (i = 0; i < POSSIBLE_JURY_IN_COURT; i++)
            sem_post(&semJury);
        sem_post(&semJudges);
    }
}
