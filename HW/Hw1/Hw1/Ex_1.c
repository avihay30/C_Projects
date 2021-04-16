#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int getLongestNegative(int[], int);
int isArrEmpty(int[], int);

#define N 8
#define TRUE 1
#define FALSE 0

int main()
{
	int arr[N], inputsNum, i;
	int tmpCounter, maxCounter = 0;
	do {
		inputsNum = FALSE;
		while (!inputsNum) { // looping if user enters a char to array.
			inputsNum = TRUE;
			printf("Enter the array: ");
			for (i = 0; i < N; i++)
				if (scanf("%d", &arr[i]) != 1)
					inputsNum = FALSE;
			if (inputsNum == FALSE) {
				rewind(stdin);
				printf("InputError: please enter %d INTEGERS seperated by spaces!\n\n", N);
			}
		}
		tmpCounter = getLongestNegative(arr, N);
		maxCounter = (maxCounter < tmpCounter) ? tmpCounter : maxCounter;
	} while (!isArrEmpty(arr, N));
	
	printf("Maximal negative sequence length is: %d\n", maxCounter);
	return 0;
}

int isArrEmpty(int arr[], int size) 
{
	int i;
	for (i = 0; i < size; i++)
		if (arr[i] != 0)
			return FALSE;
	return TRUE; // logical else.
}

int getLongestNegative(int arr[], int size) 
{
	int i, tmpCounter = 0, counter = 0;
	for (i = 0; i < size; i++) {
		if (arr[i] < 0)
			tmpCounter++;
		else {
			counter = (counter < tmpCounter) ? tmpCounter : counter;
			tmpCounter = 0;
		}
	}
	counter = (counter < tmpCounter) ? tmpCounter : counter;

	return counter;
}