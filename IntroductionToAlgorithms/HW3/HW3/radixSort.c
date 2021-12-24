#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define BASE 10
// The array elements are in range [1, 9999].
#define DIGIT_RANGE 4

int* mallocArr(int);
void radixSort(int*, int);
void countingSort(int*, int, int, int);

// Implementation of radix sort (numbers between 1-9999)
void radixSort(int* arr, int size) {
	int i;

	// Running on all digits (lsb - msb)
	for (i = 0; i < DIGIT_RANGE; i++) {
		countingSort(arr, size, BASE, i);
	}
}

// Function sorts array by digit represented by exp.
void countingSort(int* inputArr, int size, int base, int exp) {
	int i, tenExp ,digit, outputIndex;
	int* countArr;
	int* outputArr;

	tenExp = (int) pow(base, exp);
	countArr = mallocArr(base);
	outputArr = mallocArr(size);

	// Init countArr
	for (i = 0; i < base; i++) {
		countArr[i] = 0;
	}
	// Counting inputArr elements
	for (i = 0; i < size; i++) {
		digit = (inputArr[i] / tenExp) % base;
		countArr[digit]++;
	}
	// Accumulation inputArr elements
	for (i = 1; i < base; i++) {
		countArr[i] = countArr[i] + countArr[i-1];
	}
	// Filling outputArr
	for (i = size - 1; i >= 0; i--) {
		digit = (inputArr[i] / tenExp) % base;
		
		// Holds the appropriate index in sorted arr
		outputIndex = countArr[digit];
		outputArr[outputIndex - 1] = inputArr[i];
		countArr[digit]--;
	}
	// Updating inputArr to equal outputArr
	for (i = 0; i < size; i++) {
		inputArr[i] = outputArr[i];
	}
	free(countArr);
	free(outputArr);
}

void printArr(int* arr, int size) {
	int i;

	for (i = 0; i < size; i++) {
		printf("%d ", arr[i]);
	}
	printf("\n");
}

// Mallocs array of inputed size and returns it.W
int* mallocArr(int size) {
	int* arr;

	arr = (int*)malloc(sizeof(int) * size);
	if (arr == NULL) {
		printf("Allocation Error");
		return 1;
	}
	return arr;
}

int main()
{
	int i, arrSize;
	int* arr;
	printf("Enter the size of the array: ");
	scanf("%d", &arrSize);

	arr = mallocArr(arrSize);

	printf("Enter the elements of the array (in range 1-9999):\n");
	for (i = 0; i < arrSize; i++) {
		scanf("%d", &(arr[i]));
	}

	printf("Unsorted array:\n");
	printArr(arr, arrSize);

	radixSort(arr, arrSize);
	printf("Sorted array:\n");
	printArr(arr, arrSize);

	free(arr);
}