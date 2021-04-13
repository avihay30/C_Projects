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
	for (i = 0; i < N; i++) {
		printf("Enter arr[%d]: ", i);
		scanf("%d", &arr1[i]);
	}
	printf("Enter max value greater than zero: ");
	scanf("%d", &maxValue);
	while (maxValue <= 0) {
		printf("Error, Enter max value greater than zero: ");
		scanf("%d", &maxValue);
	}
	srand(time(NULL));
	for (i = 0; i < N; i++) {
		arr2[i] = (rand() % maxValue) + 1;
	}

	printf("Array 1: ");
	printArr(arr1, N);
	printf("\nArray 2: ");
	printArr(arr2, N);
	firstBiggerIndex = getFirstBiggerElementIndex(arr1, arr2, N);
	if (firstBiggerIndex != -1)
		printf("\nThe index is %d (numbers %d and %d)\n",
			firstBiggerIndex, arr1[firstBiggerIndex], arr2[firstBiggerIndex]);
	else
		printf("\nNo!!!\n");

	return 0;
}

int getFirstBiggerElementIndex(int firstArr[], int secArr[], int sizeOfArrays)
{
	int i;
	for (i = 0; i < sizeOfArrays; i++)
		if (firstArr[i] > secArr[i])
			return i;
	return -1;
}

void printArr(int arr[], int size)
{
	int i;
	for (i = 0; i < size; i++)
		printf("%d ", arr[i]);
}