#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<string.h>
#include<time.h>

#define N 10
#define M 30

int mat[N][M];

void* initMatRow(void*);
void printMat();

// The program creates N threads and calling initMatRow for each one
int main() {
	int i, ans;
	pthread_t threads[N];
	int rowNums[N];
	srand(time(NULL));

	// creating N threads
	for (i = 0; i < N; i++) {
		rowNums[i] = i;
		ans = pthread_create(&threads[i], NULL, initMatRow, (void*)&(rowNums[i]));
		if (ans != 0) {
			fprintf(stderr, "\nCan't create thread\n");
			return 0;
		}
	}
	// waiting for all threads
	for (i = 0; i < N; i++) {
		pthread_join(threads[i], NULL);
	}

	printMat();
	return 0;
}

// thread will exit in the end of the function
void* initMatRow(void* row) {
	int j, rowNum = *(int*)row;
	// filling rowNum with random values
	for (j = 0; j < M; j++) {
		mat[rowNum][j] = rand() % 101;
	}
}

// printing Matrix
void printMat() {
	int i, j;

	for (i = 0; i < N; i++) {
		for (j = 0; j < M; j++) {
			if (j == M - 1) printf("%2d", mat[i][j]);
			else printf("%2d, ", mat[i][j]);
		}
		printf("\n");
	}
}