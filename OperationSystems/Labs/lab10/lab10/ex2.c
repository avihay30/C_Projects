#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <stdlib.h>
#include <semaphore.h>
#include <string.h>

#define LEFT(i, N) (i + N - 1) % N // מקרו המגדיר את מספר השכן משמאל
#define RIGHT(i, N) (i + 1) % N // מקרו המגדיר את מספר השכן מימין
#define THINKING 0 // פילוסוף חושב
#define HUNGRY 1 // פילוסוף רעב (בהמתנה למקלות
#define EATING 2 // פילוסוף אוכל
#define MAX_TIME 5 // זמן מקסימלי שמותר לבזבז על אוכל ו/או חשיבה

void V(sem_t*);
void P(sem_t*);

int N; // מספר פילוסופים
sem_t* mutex = NULL; // מצביע לסמפור בינארי
sem_t* eaters = NULL; // מערך סמפורים של פילוסופים
int* state = NULL; // מערך שלמים לקביעת מצב פילוסוף (אוכל, רעב, חושב

// תהליך אכילה
void eat(int i) {
    int time = rand() % MAX_TIME;
    printf("Philosopher #%d is eating...\n", i + 1);
    sleep(time);
    printf("Philosopher #%d stopped eating...\n", i + 1);
}

// תהליך חשיבה
void think(int i) {
    int time = rand() % MAX_TIME;
    printf("Philosopher #%d is thinking...\n", i + 1);
    sleep(time);
}

// בדיקה האם פילוסוף רעב יכול להתחיל לאכול
void test(int i) {
    // בודקים האם הפילוסוף רעב והשכנים לא אוכלים (המקלות פנויות
    if (state[i] == HUNGRY &&
        state[LEFT(i, N)] != EATING &&
        state[RIGHT(i, N)] != EATING) {
        state[i] = EATING; // שינוי סטסוס - הפילוסוף אוכל        
        // פילוסוף התחיל לאכול, לכן הסמפור שלו עולה כדי שהשכנים שלו לא ינעלו אותו
        V(&(eaters[i]));
    }
}

// פילוסוף מנסה לקחת מקל
void take_sticks(int i) {
    P(mutex); // נכנסים לקטע קריטי ונועלים אותו כדי שהמקלות יוכל לקחת רק פילוסוף אחד
    state[i] = HUNGRY; // הפילוסוף רעב
    test(i); // מנסה לקחת 2 מקלות בעזרת פונקציית טסט
    V(mutex); // עוזבים את הקטע הקריטי
    P(&(eaters[i])); // אם לא הצליח להתחיל לאכול, ננעל. השכנים יוכלו "לפתוח" אותו אחרי שיוכלו
}

// פילוסוף מחזיר מקל
void put_sticks(int i) {
    P(mutex); //נכנסים לקטע קריטי ונועלים אותו כדי שרק פילוסוף אחד יעבוד עם המקלות
    state[i] = THINKING;   //פילוסוף סיים לאכול, התחיל לחשוב
    test(LEFT(i, N)); //בודקים האם השכן משמאל רעב, ז"א, ממתין למקל, אז נותנים לו סימן שיתחיל לאכול
    test(RIGHT(i, N)); //כנ"ל עם השכן מימין 
    V(mutex); // allow acess to mutex
}

// הקוד הזה מתבצע על ידי כל חוט המתאר התנהגות של פילוסוף
void* philosopher(void* arg) {
    int i = *((int*)arg);
    while (1 == 1) { //לולאה אין סופית
        think(i); //קודם כל הפילוסוף חושב
        take_sticks(i); //אחר כך מנסה לקחת את המקלות
        eat(i); //אוכל
        put_sticks(i); //שם את המקלות על השולחן
    }
}

int main(int argc, char* argv[]) {
    void* result;
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <NUMBER_OF_PHILOSOPHERS>\n", argv[0]);
        return 1;
    }
    //הכנה...
    N = atoi(argv[1]); //המרת כמות הפילוסופים ממחרוזת למספר שלם
    if (N <= 1) {
        fprintf(stderr, "Error by transformation of the argument...\n");
        return 2;
    }//לא פחות מ-2 פילוסופים

    mutex = (sem_t*)malloc(sizeof(sem_t));
    if (sem_init(mutex, 0, 1) != 0) { //יוצרים מיוטקס להפרדה בכניסה לקטע קריטי
        fprintf(stderr, "Error by creating semaphore...\n");
        return 3;
    }

    eaters = (sem_t*)malloc(N * sizeof(sem_t)); //מקצים זיכרון לכמות סמפורים - לכל פילוסוף סמפור
    state = (int*)malloc(N * sizeof(int)); // (מקצים זיכרון למערך שלמים, כל שלם מייצג מצב פילוסוף מתאים (רעב, אוכל, חושב    

    memset(state, 0, N);

    srand(time(NULL));
    pthread_t* philosophers = (pthread_t*)malloc(N * sizeof(pthread_t)); //מקצים זיכרון לחוטים לפי כמות הפילוסופים

    int i;
    for (i = 0; i < N; i++) {
        if (sem_init(&eaters[i], 0, 0) != 0) { //מאתחלים סמפורים של פילוסופים
            fprintf(stderr, "Error by creating semaphore...\n");
            return 3;
        }
    }

    for (i = 0; i < N; i++) {
        if (pthread_create(&philosophers[i], NULL, philosopher, (void*)&i) != 0) {
            fprintf(stderr, "Error by creating thread\n");
            return 2;
        }
        usleep(100000);
    }

    for (i = 0; i < N; i++) {
        if (pthread_join(philosophers[i], &result) != 0) {
            fprintf(stderr, "Error by joining thread\n");
            return 3;
        }
    }
    return 0;
}

void V(sem_t* sem) {
    sem_post(sem);
}
void P(sem_t* sem) {
    sem_wait(sem);
}
