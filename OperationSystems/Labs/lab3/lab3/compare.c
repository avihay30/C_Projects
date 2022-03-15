#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<string.h>

#define BUFFER_SIZE 256

void Error(char* msg) {
    fprintf(stderr, "Error:%s\n", msg);
    exit(1);
}

// closing both given fd1, fd2
void closeFds(int fd1, int fd2) {
    close(fd1);
    close(fd2);
}

// check if argv[1] is identical to argv[2]
// both file are exists
int main(int argc, char* argv[]) {
    FILE* out = stdout;
    int fd1, fd2, r1bytes, r2bytes;
    char str1[BUFFER_SIZE] = {'\0'}, str2[BUFFER_SIZE] = {'\0'};

    if (argc != 3) Error("Invalid given arguments, should exactly two files given\n");

    // opening both files for read-only
    if ((fd1 = open(argv[1], O_RDONLY)) == -1) {
        perror("open first file"); return(-1);
    }
    if ((fd2 = open(argv[2], O_RDONLY)) == -1) {
        perror("open second file"); return(-1);
    }

    // iterering over both files
    do {
        // reding from both files to buffes(str1, str2)
        if ((r1bytes = read(fd1, str1, BUFFER_SIZE - 1)) == -1) {
            perror("read 1"); return(-1);
        }
        if ((r2bytes = read(fd2, str2, BUFFER_SIZE - 1)) == -1) {
            perror("read 2"); return(-1);
        }
        // compering read sizes and read values (ch1==ch2)
        if (r1bytes != r2bytes || strcmp(str1, str2) != 0) {
            fprintf(out, "Different files\n");
            closeFds(fd1, fd2);
            return 0;
        }
    // continue until reading both files has ended ("empty")
    } while (r1bytes > 0 && r2bytes > 0);

    fprintf(out, "Identical files\n");
    closeFds(fd1, fd2);
    return 0;
}