#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void Error(char* msg) {
    fprintf(stderr, "Error:%s\n", msg);
    exit(1);
}

int main(int argc, char* argv[]) {
    FILE* out = stdout;
    int i, sum = 0;
    float avg;

    if (argc == 1) Error("Invalid given arguments, should be at least one number\n");

    // iterating over all input args
    for (i = 1; i < argc; i++) {
        sum += atoi(argv[i]);
    }
    avg = (float)sum / (argc - 1);
    fprintf(out, "%.2f\n", avg);

    return 0;
}