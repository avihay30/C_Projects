#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

void sys_err(char str[]) { perror(str); exit(2); }

// Program to simulate the command of `date | wc`
// using pipe between proccess
int main() {
	int pfd[2];

	if (pipe(pfd) == -1) sys_err("pipe");

	switch (fork()) {
	case -1: // fork failed
		perror("fork");
		return 1;
	case 0: // only child execute this
		// moving stdout to be pfd[0]
		if (dup2(pfd[1], 1) == -1) sys_err("dup stdout");
		close(pfd[0]); // closing read in pipe
		execlp("date", "date", 0);
		perror("execlp");
		return 1;
	default:
		wait(); // waiting for child to print "a"
		// moving stdin to be pfd[0]
		if (dup2(pfd[0], 0) == -1) sys_err("dup stdin");
		close(pfd[1]); // closing write in pipe
		printf("In date there are:\nLine:   Words:   Chars:\n");
		execlp("wc", "wc", 0);
		perror("execlp");
		break;
	} /* switch */
	return 0;
}