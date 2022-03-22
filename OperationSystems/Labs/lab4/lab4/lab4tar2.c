#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<string.h>

#define BUFFER_SIZE 256
#define EXIT_COMMEND "q"

// running demo shell that exits when q is entered
int main(int argc, char* argv[]) {
    char commend[BUFFER_SIZE];

    if (argc != 1) {
        fprintf(stderr, "Error:Invalid given arguments, should not get arguments\n");
        return 1;
    }

    while (1) {
        fprintf(stdout, "$");
        fgets(commend, BUFFER_SIZE, stdin);
        // remove "\n" from the end
        commend[strlen(commend) - 1] = '\0';

        // checking if user input is "q"
        if (strcmp(commend, EXIT_COMMEND) == 0) {
            fprintf(stdout, "END\n");
            return 0;
        }

        switch (fork()) {
        // if fork failed
        case -1:
            perror("fork"); return(-1);
        // Only child execute this
        case 0:
            // executing bin commend
            execlp(commend, commend, 0);
            fprintf(stderr, "ERROR ERROR ERROR\n");
            // if child has not done any commend 
            return -1;
        // Only parent execute this
        default:
            waitpid(-1, NULL, 0);
            break;
        }
    }
    return 0;
}