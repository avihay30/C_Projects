#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

void createHistogram(char* str);
void printHistogram(int[], int);

#define N 6
#define STR_SIZE 40

int main()
{
	char str[STR_SIZE + 1], chr;
	int i;
	printf("Enter your string: ");
	for (i = 0; i < STR_SIZE && str[i - 1] != '!'; i++) {
		scanf("%c", &str[i]);
	}
	str[i] = '\0'; // ending string with '\0'
	if (str[strlen(str) - 1] != '!') {
		printf("InputError: Histogram could not be created. invalid ending of string, should be ended with '!'\n");
		return 1;
	}

	createHistogram(str);
	return 0;
}

void createHistogram(char *str)
{
	int cntArray[N] = { 0 }, indexInAZ;
	for (; *str != '\0' && *str != '!'; str++) {
		indexInAZ = *str - 'a';
		if (indexInAZ >= 0 && indexInAZ < N)
			cntArray[indexInAZ]++;
	}
	printHistogram(cntArray, N);
}

void printHistogram(int arr[], int size)
{
	int i, j;
	for (i = 0; i < size; i++) {
		printf("%c: ", i + 'a');
		for (j = 0; j < arr[i]; j++)
			printf("*");
		printf("\n");
	}
}