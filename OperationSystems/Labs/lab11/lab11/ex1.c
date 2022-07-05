#include <stdio.h>

// program that child always prints "a", and father prints "b"
int main() {
	while (1) {
		switch (fork()) {
		case -1: // fork failed
			perror("fork");
			return 1;
		case 0: // only child execute this
			printf("a\n"); // child prints and finish
			return 1;
		default:
			wait(); // waiting for child to print "a"
			printf("b\n");
			break;
		} /* switch */
	}

	return 0;
}