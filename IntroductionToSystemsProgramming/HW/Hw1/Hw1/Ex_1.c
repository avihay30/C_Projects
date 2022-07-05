#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int getLongestNegative(int[], int);
int isArrEmpty(int[], int);

#define N 8
#define TRUE 1 // defining true/false for readability
#define FALSE 0

int main()
{
	int arr[N], isInputValid, i;
	int tmpCounter, maxCounter = 0;
	do { // running until array is full of zeros.
		isInputValid = FALSE; // a flag to exit inside loop if user try to enter char to array.
		while (!isInputValid) { // looping if user enters a char to array.
			isInputValid = TRUE;
			printf("Enter the array: ");
			for (i = 0; i < N; i++)
				if (scanf("%d", &arr[i]) != 1) // checking if invalid input.
					isInputValid = FALSE;
			if (isInputValid == FALSE) {
				rewind(stdin); // cleaning buffer of keyboard after user inserts a char.
				printf("InputError: please enter %d INTEGERS seperated by spaces!\n\n", N);
			}
		}
		tmpCounter = getLongestNegative(arr, N);
		// "maxCounter" holds always the biggest negative sequence possible. 
		maxCounter = (maxCounter < tmpCounter) ? tmpCounter : maxCounter;
	} while (!isArrEmpty(arr, N));
	
	printf("Maximal negative sequence length is: %d\n", maxCounter);
	return 0;
}

/* Function gets a pointer to an array and it's size
   and returns wethere the array is full of zeros or not. */
int isArrEmpty(int arr[], int size) 
{
	int i;
	for (i = 0; i < size; i++) // looping the array.
		if (arr[i] != 0)
			return FALSE;
	return TRUE; // logical else.
}

/* Function gets a pointer to an array and it's size
   and returns the size of the longest negative sequence of that array. */
int getLongestNegative(int arr[], int size) 
{
	int i, tmpCounter = 0, counter = 0;
	for (i = 0; i < size; i++) { // looping the array.
		if (arr[i] < 0)
			tmpCounter++;
		else {
			// inserting counter the max("counter", "tmpCounter")
			counter = (counter < tmpCounter) ? tmpCounter : counter;
			tmpCounter = 0;
		}
	}
	// re-assigning counter to the last sequence size is in the end of the array if it's bigger.
	counter = (counter < tmpCounter) ? tmpCounter : counter;

	return counter;
}