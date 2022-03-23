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
char* getCurrentWord(char*);
int isEnclosingQuote(char);
int isDelimeter(char);
void initParams(char*[NUM_OF_PARAMS]);
void freeAllParams(char*[NUM_OF_PARAMS]);

/* Shell that supports original bin commends
   and is extended with some other "new" commends that exists on the same dir */
int main(int argc, char* argv[]) {
    char newCommend[BUFFER_SIZE];
    char commend[BUFFER_SIZE];
    char* params[NUM_OF_PARAMS];
    // -1 returned value (=255), in 3 bits (for returned value from WEXITSTATUS)
    int errExitStatus = 255;
    int returnStatus = -1;

    if (argc != 1) Error("Invalid given arguments, should not get arguments\n");

    while (1) {
        newCommend[0] = '\0'; commend[0] = '\0';
        initParams(params);
        displayPrompt();
        readCommend(commend, params);
        // skipping empty line
        if (strlen(commend) == 0) continue;

        if (strcmp(commend, EXIT_COMMEND) == 0) {
            printf("GoodBye...\n"); return 0;
        }

        switch (fork()) {
        // if fork failed
        case -1:
            perror("fork"); return(-1);
        // Only child execute this
        case 0:
            // executing bin commend (if possible)
            // p is for $PATH, shortcut for "/bin/..."
            execvp(commend, params);

            // executing new commend (if bin commend failed)
            sprintf(newCommend, "./%s", commend);
            execv(newCommend, params);

            // if child has not done any commend
            fprintf(stderr, "Not Supported\n"); 
            return -1;
        // Only parent execute this
        default:
            waitpid(-1, &returnStatus, 0);
            returnStatus = WEXITSTATUS(returnStatus);
            // if getPrice process has been ran, 
            // printing the returned price (if successful)
            if (strcmp(commend, "getPrice") == 0 && returnStatus != errExitStatus) {
                printf("%d NIS\n", returnStatus);
            }
            freeAllParams(params);
            break;
        }
    }
    return 0;
}

// reset all param to NULL
void initParams(char* params[NUM_OF_PARAMS]) {
    int i;
    for (i = 0; i < NUM_OF_PARAMS; i++) params[i] = NULL;
}

void Error(char* msg) {
    fprintf(stderr, "Error:%s\n", msg);
    exit(1);
}

void displayPrompt() {
    printf("AdvShell>");
}

/* Function reads user inputand parse it into commendand his params,
   insert the parsed commend into the given param of the function */
void readCommend(char* commend, char* params[NUM_OF_PARAMS]) {
    int currCharIdx = 0, paramIdx = 0, isEnclosed = 0;
    char buffer[BUFFER_SIZE] = { '\0' };
    char someParam[BUFFER_SIZE] = { '\0' };
    char* tempStr;

    fgets(buffer, BUFFER_SIZE, stdin);
    // walk through each words in inputed buffer
    // until next char is emptyLine (line ended)
    while (buffer[currCharIdx] != '\n') {
        // handling case of spaces between input params
        if (isDelimeter(buffer[currCharIdx])) currCharIdx++;
        else {
            // setting tempStr to hold rest of buffer
            strcpy(someParam, buffer + currCharIdx);
            isEnclosed = isEnclosingQuote(*someParam);
            // tempStr will hold the first word until some delimeter
            tempStr = getCurrentWord(someParam);
            // skipping first quote
            if (isEnclosed) tempStr = someParam + 1;
            // setting first word to be the commend
            if (paramIdx == 0) strcpy(commend, tempStr);

            params[paramIdx] = (char*)malloc(strlen(tempStr));
            if (params[paramIdx] == NULL) Error("Allocation Error");
            strcpy(params[paramIdx++], tempStr);

            // moving index in order to find the next word, if exists
            currCharIdx += strlen(tempStr);
            // if commend had Quotes skipping them
            if (isEnclosed) currCharIdx += 2;
        }
    }
}

// returns trim current word (remove white spaces)
char* getCurrentWord(char* str) {
    int i = 0;
    // checking if word is enclosed with "__" or '__'
    if (isEnclosingQuote(*str)) {
        i++;
        // removing first show of "__" or '__'
        str++;
        while (!isEnclosingQuote(*(str + i))) i++;
    // handling case of regular word, stops at some delimeter
    }
    else {
        while (!isDelimeter(*(str + i))) i++;
    }
 
    *(str + i) = '\0';
    return str;
}

// checking if given char is some kind of a quote
int isEnclosingQuote(char ch) {
    char quote[2] = "'";
    if (ch == '"' || ch == quote[0]) return 1;
    return 0;
}

// returns true if the specified char is a delimiter.
int isDelimeter(char ch) {
    int i;
    int amountOfDelim = 3;
    char delimeters[] = { ' ', '\n', '\t' };

    for (i = 0; i < amountOfDelim; i++) {
        if (delimeters[i] == ch) return 1;
    }
    return 0;
}

void freeAllParams(char* params[NUM_OF_PARAMS]) {
    int i;
    for (i = 0; i < NUM_OF_PARAMS; i++) free(params[i]);
}