#include<stdio.h>
#include<unistd.h>

#define N 10

// printing fibo series of N, by using processes
int main() {
	int c1, c2, t, i;
	c2 = 1; c1 = 0;

	for (i = 1; i < N; i++) {
		switch (fork()) {
		case -1: // if fork failed
			printf("Error creating process\n");
			return -1;
		case 0: // child enter here
			t = c1; // calculating the next fibo
			c1 = c2;
			c2 += t;
			break;
		default: // father enter here
			// stops the loop, 
			// father needs to create only one child
			i = N;
			break;
		}
	}
	wait(); // waiting for child to finish
	printf("%d ", c2);
	// if it's the last print. (root father)
	if (c1 == 0 && c2 == 1) printf("\n");
	return 0;
}