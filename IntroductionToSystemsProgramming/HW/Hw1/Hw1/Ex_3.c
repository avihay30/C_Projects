#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

void createHistogram(char* str);
void printHistogram(int[], int);

#define N 4
#define STR_SIZE 40

int main()
{
	char str[STR_SIZE], chr;
	int i;
	printf("Enter your string: ");
	// iterating until user enter a '!' and there is still free space in string.
	for (i = 0; i < STR_SIZE && str[i - 1] != '!'; i++) {
		scanf("%c", &str[i]);
	}
	if (str[i - 1] != '!') { // if last char in string is not '!'.
		printf("InputError: Histogram could not be created.\n");
		printf("Invalid ending of string with max size of %d, should be ended with '!'\n", STR_SIZE);
		return 1;
	}

	createHistogram(str);
	return 0;
}

/* Function gets a pointer to a string
   and builds a counter array of size N of occurrences of chars in string,
   and print the result by calling anther function. */
void createHistogram(char *str)
{
	int cntArray[N] = { 0 }, indexInAZ;
	// iterating over the string until *str == '!'.
	for (; *str != '!'; str++) {
		indexInAZ = *str - 'a'; // calculating index of *str in a-z.
		if (indexInAZ >= 0 && indexInAZ < N) // checking if valid to insert into histogram.
			cntArray[indexInAZ]++;
	}
	printHistogram(cntArray, N);
}

/* Function gets a pointer to array and it's size,
   and prints the array according to the requirements. */
void printHistogram(int arr[], int size)
{
	int i, j;
	for (i = 0; i < size; i++) { // itereting over elements in arr.
		printf("%c: ", i + 'a'); // prints the char of that row.
		for (j = 0; j < arr[i]; j++) // itereting arr[i] times and adding * each iteration.
			printf("*");
		printf("\n"); // space between rows.
	}
}