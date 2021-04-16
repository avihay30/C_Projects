#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>

void printArray(long arr[], int size);
int getFirstDigit(long num);
void buildNum(long* src, int firstdigit);
void leftCircularShift(long arr[], int size);

#define N 4
#define TRUE 1
#define FALSE 0

int main()
{
	int i, inputsNum;
	long arr[N];

	inputsNum = FALSE;
	while (!inputsNum) { // looping if user enters a char to array.
		inputsNum = TRUE;
		printf("Please enter an array of %d long values: ", N);
		for (i = 0; i < N; i++)
			if (scanf("%d", &arr[i]) != 1)
				inputsNum = FALSE;
		if (inputsNum == FALSE) {
			rewind(stdin);
			printf("InputError: please enter %d LONGS seperated by spaces!\n\n", N);
		}
	}
	printf("\nArray before shift: ");
	printArray(arr, N);

	leftCircularShift(arr, N);
	printf("\nArray after shift: ");
	printArray(arr, N);
	printf("\n");

	return 0;
}

void printArray(long arr[], int size)
{
	int i;
	for (i = 0; i < size; i++)
		printf("%d ", arr[i]);
}

int getFirstDigit(long num)
{
	long divider = (long) pow(10, (int) log10(num));
	return num / divider;
}

void buildNum(long *src, int firstdigit)
{
	// casting to int because the modulo operand afterwards.
	long divider = (long) pow(10, (int) log10(*src));
	// deleting first digit.
	*src = *src % divider;
	// adding digit to the right.
	*src *= 10;
	*src += firstdigit;
}

void leftCircularShift(long arr[], int size)
{
	int i;
	long firstDigitOfFirst;
	for (i = 0; i < size - 1; i++) {
		if (i == 0)
			firstDigitOfFirst = getFirstDigit(arr[i]);
		// building number.
		buildNum(&arr[i], getFirstDigit(arr[i + 1]));
	}
	// building last number.
	if (size > 1)
		buildNum(&arr[size - 1], firstDigitOfFirst);
}