#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>

// running argv[1] commend argv[2] times with different processes
void main(int argc, char* argv[]) {
    int i, numOfIterations = atoi(argv[2]);
    char commandPath[256];

	if (argc != 3) {
		fprintf(stderr, "Error: Invalid given arguments, should exactly two given\n");
		return 1;
	}
	// adding prefixed path to commend
    sprintf(commandPath, "./%s", argv[1]);
	for (i = 0; i < numOfIterations; i++) {
        switch (fork()) {
		case -1: // fork failed
			perror("fork");
			return 1;
		case 0: // only child execute this
			execl(commandPath, argv[1], 0);
			perror("execl");
			return 1;
		default:
			break;
		} /* switch */
	}
	for (i = 0; i < numOfIterations; i++) {
		wait();
	}
	puts("END");
}