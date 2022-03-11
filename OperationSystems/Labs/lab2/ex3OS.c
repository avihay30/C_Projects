#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void Error(char* msg) {
    fprintf(stderr, "Error:%s\n", msg);
    exit(1);
}

/* Convert the first arg in argv and split it with '@' as a delimeter,
   then print it in uppercase letters. */
int main(int argc, char* argv[]) {
    FILE* out = stdout;
    int i;
    char currChar;

    if (argc != 2) Error("Invalid given arguments, should be only ONE\n");

    // iterating over all chars in first argument
    for (i = 0; i < strlen(argv[1]); i++) {
        currChar = argv[1][i];

        if (currChar == '@') {
            currChar = '\n';
        }
        // if char is a letter => do upperCase
        else if (currChar >= 'a' && currChar <= 'z') {
            currChar -= 32;
        }
        fprintf(out, "%c", currChar);
    }
    fprintf(out, "\n");

    return 0;
}