#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<string.h>

void Error(char* msg) {
	fprintf(stderr, "Error:%s\n", msg);
	exit(1);
}

int copy(char* from, char* to) {
	int fd_from, fd_to, rbytes, wbytes; // where a pointer that looks at an item is saved 
	char buff[256];
	if ((fd_from = open(from, O_RDONLY)) == -1)
	{
		perror("open from"); return(-1);
	}
	if ((fd_to = open(to, O_WRONLY | O_CREAT, 0664)) == -1)
	{
		perror("open to"); return(-1);
	}
	if ((rbytes = read(fd_from, buff, 256)) == -1)
	{
		perror("read 1"); return(-1);
	}
	while (rbytes > 0) {
		if ((wbytes = write(fd_to, buff, rbytes)) == -1)
		{
			perror("write"); return(-1);
		}
		if (wbytes != rbytes)
		{
			fprintf(stderr, "bad write\n"); return(-1);
		}
		if ((rbytes = read(fd_from, buff, 256)) == -1)
		{
			perror("read 2"); return(-1);
		}
	}
	close(fd_from); close(fd_to);  return(0);
}

// Creating folder given in argv[1] 
// and creating copied file (from argv[2]) to the new directory
int main(int argc, char* argv[]) {
	char fileInDirPath[256];

	if (argc != 3) Error("Invalid given arguments, should exactly two files given\n");
	// creating new directory
	if (mkdir(argv[1], 0777) == -1) {
		perror("create folder"); return(-1);
	}

	strcpy(fileInDirPath, argv[1]);
	strcat(fileInDirPath, "/");
	strcat(fileInDirPath, argv[2]);

	return copy(argv[2], fileInDirPath);
}
