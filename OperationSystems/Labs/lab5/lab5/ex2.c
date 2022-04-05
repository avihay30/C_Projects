#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<string.h>
#include<unistd.h>

unsigned long int counter;

void* incCount();
void* printCount();

/* creating 2 threads, one for incrementing global `counter`
   and other for printing it */
int main() {
	pthread_t threads[2];
	int ans1, ans2;

	ans1 = pthread_create(&threads[0], NULL, incCount, NULL);
	ans2 = pthread_create(&threads[1], NULL, printCount, NULL);
	if (ans1 != 0 || ans2 != 0) {
		fprintf(stderr, "\nCan't create threads\n");
		return 0;
	}

	pthread_join(threads[1], NULL);
	return 0;
}

void* incCount() {
	while (1) counter++;
}

void* printCount() {
	int i;

	// printing 10 for one second
	for (i = 0; i < 10; i++) {
		usleep(100000); // sleeping for 0.1 sec
		printf("%d\n", counter);
	}
}