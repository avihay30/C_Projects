#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>

void printArray(long arr[], int size);
int getFirstDigit(long num);
void buildNum(long* src, int firstdigit);
void leftCircularShift(long arr[], int size);

#define N 4
#define TRUE 1 // defining true/false for readability
#define FALSE 0

int main()
{
	int i, isInputValid;
	long arr[N];

	isInputValid = FALSE; // a flag to exit inside loop if user try to enter char to array.
	while (!isInputValid) { // looping if user enters a char to array.
		isInputValid = TRUE;
		printf("Please enter an array of %d long values: ", N);
		for (i = 0; i < N; i++)
			if (scanf("%d", &arr[i]) != 1) // checking if invalid input.
				isInputValid = FALSE;
		if (isInputValid == FALSE) {
			rewind(stdin); // cleaning buffer of keyboard after user inserts a char.
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

/* Function gets a pointer to array and it's size
   and prints all elements of array in one line. */
void printArray(long arr[], int size)
{
	int i;
	for (i = 0; i < size; i++)
		printf("%d ", arr[i]);
}

// Function gets a long number and returns the first digit(leftest)
int getFirstDigit(long num)
{
	/* (int)log10(number) + 1 is equal to numberOfDigits.
	   calculating by using pow with log10,
	   (e.x: for num = 234: divder = 10 ^ 2 = 100 )) */
	long divider = (long) pow(10, (int) log10(num));
	// returning one digit(e.x: (int) 234 / 100 = 2).
	return (int) num / divider;
}

/* Function get a pointer to a long number and a digit 
   and adds the "firstdigit" to the right of the long number. */
void buildNum(long *src, int firstdigit)
{
	// casting to long because the modulo operand afterwards.
	long divider = (long) pow(10, (int) log10(*src));
	// deleting first digit.
	*src = *src % divider;
	// adding digit to the right.
	*src *= 10;
	*src += firstdigit;
}

/* Function gets a pointer to an array and it's size,
   and doing the leftCircularShift operation on the array. */
void leftCircularShift(long arr[], int size)
{
	int i;
	/* declaring an int that will hold the leftest digit 
	   of the first element in the array. */
	long firstDigitOfFirst; 
	for (i = 0; i < size - 1; i++) { // iterating the array.
		if (i == 0)
			firstDigitOfFirst = getFirstDigit(arr[i]);
		// building number.
		buildNum(&arr[i], getFirstDigit(arr[i + 1]));
	}
	// building last number with "firstDigitOfFirst" variable.
	if (size > 1)
		buildNum(&arr[size - 1], firstDigitOfFirst);
}