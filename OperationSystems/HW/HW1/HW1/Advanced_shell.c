#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<string.h>

#define BUFFER_SIZE 256
// 1 -> commend, [2,4] -> params, [5] -> NULL (end of argv)
#define NUM_OF_PARAMS 5
#define EXIT_COMMEND "exit"

void Error(char*);
void displayPrompt();
void readCommend(char*, char*[NUM_OF_PARAMS]);
void removeEnding(char*);
void initParams(char* [NUM_OF_PARAMS]);

int main(int argc, char* argv[]) {
    char binCommend[BUFFER_SIZE] = "/bin/";
    char newCommend[BUFFER_SIZE] = "./";
    char commend[BUFFER_SIZE];
    char* params[NUM_OF_PARAMS];

    if (argc != 1) Error("Invalid given arguments, should not get arguments\n");

    while (1) {
        initParams(params);
        displayPrompt();
        readCommend(commend, params);

        if (strcmp(commend, EXIT_COMMEND) == 0) {
            fprintf(stdout, "GoodBye...\n");
            return 0;
        }

        switch (fork()) {
        // if fork failed
        case -1:
            perror("fork"); return(-1);
        // Only child execute this
        case 0:
            // executing bin commend (if possible)
            strcat(binCommend, commend);
            strcpy(params[0], binCommend);
            execvp(binCommend, params);

            // executing new commend (if bin commend failed)
            strcat(newCommend, commend);
            strcpy(params[0], newCommend);
            execvp(newCommend, params);
            fprintf(stdout, "Not Supported\n");
            // if child has not done any commend 
            return -1;
        // Only parent execute this
        default:
            waitpid(-1, NULL, 0);
            break;
        }
    }
}

void initParams(char* params[NUM_OF_PARAMS]) {
    int i;
    for (i = 0; i < NUM_OF_PARAMS; i++) params[i] = NULL;
}

void Error(char* msg) {
    fprintf(stderr, "Error:%s\n", msg);
    exit(1);
}

void displayPrompt() {
    fprintf(stdout, "AdvShell>");
}

void readCommend(char* commend, char* params[NUM_OF_PARAMS]) {
    int i = 1;
    char buffer[BUFFER_SIZE] = { '\0' };
    char delimeter[2] = " ";
    char* token;
    fgets(buffer, BUFFER_SIZE, stdin);
    removeEnding(buffer);

    // get the first token
    token = strtok(buffer, delimeter);
    strcpy(commend, token);

    params[0] = (char*)malloc(strlen(commend));
    if (params[0] == NULL) Error("Allocation Error");

    strcpy(params[0], commend);

    // walk through other tokens, fill NULLs in params array is not exist.
    while (i < NUM_OF_PARAMS - 1) {
        token = strtok(NULL, delimeter);
        // if no params are left
        if (token == NULL) return;

        params[i] = (char*)malloc(strlen(token));
        if (params[i] == NULL) Error("Allocation Error");
        strcpy(params[i++], token);
    }
}

void removeEnding(char* str) {
    str[strlen(str) - 1] = '\0';
}