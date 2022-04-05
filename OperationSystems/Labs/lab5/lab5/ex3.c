#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<string.h>
#include<time.h>

#define N 10

int arr[N];
// default value is false (i.g not found)
int isFound = 0;
int numToFind;

void* findNum(void*);
void printArr();

/* Program that gets int from user input 
   and check if that int is in the random array using 2 threads */
int main() {
	int i, ans1, ans2;
	int zero = 0, one = 1;
	pthread_t threads[2];

	// init arr with random values
	srand(time(NULL));
	for (i = 0; i < N; i++) {
		arr[i] = rand() % 101;
	}

	// getting guess from user
	printf("Input your guess [0, 100]: ");
	scanf("%d", &numToFind);

	ans1 = pthread_create(&threads[0], NULL, findNum, (void*)&zero);
	ans2 = pthread_create(&threads[1], NULL, findNum, (void*)&one);
	if (ans1 != 0 || ans2 != 0) {
		fprintf(stderr, "\nCan't create threads\n");
		return 0;
	}

	pthread_join(threads[0], NULL);
	pthread_join(threads[1], NULL);

	// printing result
	printf("\nThe number %d has %sbeen found in:\n", numToFind, isFound ? "" : "*not* ");
	printArr();
	return 0;
}

// If global `numToFind` in the arr setting global `isFound` to 1
void* findNum(void* threadNum) {
	int i, tNum = *(int*)threadNum;

	// running from 0 (if thread is 0) else starting from 1
	// while num was not already found (i.g by other thread)
	for (i = tNum; i < N && !isFound; i += 2) {
		if (arr[i] == numToFind) {
			isFound = 1;
			return;
		}
	}
}

void printArr() {
	int i;
	printf("[");
	for (i = 0; i < N; i++) {
		if (i == N - 1) printf("%d]\n", arr[i]);
		else printf("%d, ", arr[i]);
	}
}