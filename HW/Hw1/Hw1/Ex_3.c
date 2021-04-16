#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

void incrementHistogram(int[], int, char);
void printHistogram(int[], int);

#define N 4
#define TRUE 1
#define FALSE 0

int main()
{
	char *str;
/*	str = (char*)(mall)
	int cntArray[N], numOfChars = 0;
	printf("Enter your string: ");
	do {
		scanf("%c", &chr);
		numOfChars++;

		
	} while (chr != "!");
	*/
	return 0;
}

void incrementHistogram(int arr[], int size, char chr)
{
	int indexInAZ = chr - 'a';
	if (indexInAZ <= size)
		arr[indexInAZ]++;
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