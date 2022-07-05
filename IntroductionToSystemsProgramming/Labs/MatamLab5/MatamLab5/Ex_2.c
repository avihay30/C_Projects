#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int getFirstBiggerElementIndex(int[], int[], int);
void printArr(int[], int);

#define N 7

int main()
{
	int arr1[N], arr2[N];
	int i, maxValue, firstBiggerIndex;
	for (i = 0; i < N; i++) { // getting inputs from user and filling arr1.
		printf("Enter arr[%d]: ", i);
		scanf("%d", &arr1[i]);
	}
	printf("Enter max value greater than zero: ");
	scanf("%d", &maxValue);
	while (maxValue <= 0) { // asking again and again for > 0 value.
		printf("Error, Enter max value greater than zero: ");
		scanf("%d", &maxValue);
	}
	srand(time(NULL)); // setting the random to be generate different numbers every call.
	for (i = 0; i < N; i++) { // inserting "N" random values with max of "maxValue".
		arr2[i] = (rand() % maxValue) + 1;
	}

	printf("Array 1: ");
	printArr(arr1, N);
	printf("\nArray 2: ");
	printArr(arr2, N);
	firstBiggerIndex = getFirstBiggerElementIndex(arr1, arr2, N);
	if (firstBiggerIndex != -1) // if there is arr1[i] > arr2[i]
		printf("\nThe index is %d (numbers %d and %d)\n",
			firstBiggerIndex, arr1[firstBiggerIndex], arr2[firstBiggerIndex]);
	else
		printf("\nNo!!!\n");

	return 0;
}

/* Function gets two pointers to arrays and size of them
   and checks if a condition is met(arr1[i] > arr2[i]),
   if so, returns the index. if not returns -1. */
int getFirstBiggerElementIndex(int firstArr[], int secArr[], int sizeOfArrays)
{
	int i;
	for (i = 0; i < sizeOfArrays; i++)
		if (firstArr[i] > secArr[i])
			return i;
	return -1;
}

/* Function gets a pointer to array and size, and prints it's values. */
void printArr(int arr[], int size)
{
	int i;
	for (i = 0; i < size; i++)
		printf("%d ", arr[i]);
}