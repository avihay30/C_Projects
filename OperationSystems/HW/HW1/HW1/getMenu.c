#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<string.h>

#define BUFFER_SIZE 256

void Error(char*);

/* argv[1] should be restaurant name
   prints menu of specified restaurant */
int main(int argc, char* argv[]) {
	int fd_from, rbytes, wbytes;
	int fdStdout = 1;
	char resFileName[BUFFER_SIZE];
	char buff[BUFFER_SIZE];

	if (argc != 2) Error("Invalid given arguments, should exactly one given\n");

	// adding filename extension (BBB -> BBB.txt)
	sprintf(resFileName, "%s.txt", argv[1]);
	if ((fd_from = open(resFileName, O_RDONLY)) == -1) {
		perror("open restaurant menu"); return -1;
	}

	// reading from opened restaurant menu file and writing to stdout(screen)
	if ((rbytes = read(fd_from, buff, BUFFER_SIZE)) == -1) {
		perror("read restaurant menu"); return -1;
	}
	while (rbytes > 0) {
		if ((wbytes = write(fdStdout, buff, rbytes)) == -1) {
			perror("write menu"); return -1;
		}
		if (wbytes != rbytes) {
			fprintf(stderr, "bad write\n"); return -1;
		}
		if ((rbytes = read(fd_from, buff, BUFFER_SIZE)) == -1) {
			perror("read1 restaurant menu"); return -1;
		}
	}
	close(fd_from);
	return 0;
}

void Error(char* msg) {
    fprintf(stderr, "Error:%s\n", msg);
    exit(1);
}