#include<stdio.h>
#include<math.h>
#include<pthread.h>

void* calcDivisiors(void*);

int n;
int isPrime = 1;

// printing all divisors of given argv[1]
// by using 2 theards
int main(int argc, char* argv[]) {
	int ans1, ans2, zero = 0, one = 1;
	int i, numOfThreads = 2;
	pthread_t threads[numOfThreads];

	if (argc != 2) { // if invalid amount of args
		fprintf(stderr, "No valid amount of arguments, should be only 2\n");
		return 1;
	}
	n = atoi(argv[1]);

	ans1 = pthread_create(&threads[0], NULL, calcDivisiors, (void*)&(zero));
	ans2 = pthread_create(&threads[1], NULL, calcDivisiors, (void*)&(one));
	
	if (ans1 != 0 || ans2 != 0) {
		fprintf(stderr, "\nCan't create threads\n");
		return 0;
	}

	// waiting for all threads to finish.
	for (i = 0; i < numOfThreads; i++) {
		pthread_join(threads[i], NULL);
	}

	if (isPrime) // if no divisor has been found
		printf("%d is prime number\n", n);
	return 0;
}


// function gets 0 or 1 as argument
// and calculate the odd/even number up to sqrt(n)
// that are divisors of n
void* calcDivisiors(void* zeroOrOne) {
	int i, isOdd, upperBound;
	int div2;
	isOdd = *(int*)zeroOrOne;
	upperBound = (int) sqrt(n);

	for (i = isOdd + 2; i <= upperBound; i += 2) {
		if (n % i == 0) { // if i is a divisor
			// if it's the first divisor that has been found
			if (isPrime) {
				printf("Divisors of %d:\n", n);
				isPrime = 0;
			}
			// printing the divisor and it's complement mult to n.
			printf("%d\n", i);
			div2 = (int)(n / i);
			if (i != div2) // not printing dup (e.g i=3, n=9)
				printf("%d\n", div2);
		}
	}
}