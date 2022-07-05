#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

double* inputArithmetic(double* a1, double* d, int* n);
double setArithmetic(double a1, double d, double* arr, int n);

int main()
{
	double a1, d, *arr, sum;
	int n = 1, i;
	do { // do while loop until n <= 0
		arr = inputArithmetic(&a1, &d, &n); // calling function to get inputs.
		// function "inputArithmetic" returns NULL if n <= 0.
		if (arr != NULL) {
			sum = setArithmetic(a1, d, arr, n); // setting the array with values.
			printf("\n");
			for (i = 0; i < n; i++) {
				printf("%10lf  ", arr[i]); // %10lf adding padding of 2 (10 - 7[digits] - 1['.']).
				if ((i + 1) % 5 == 0) // printing maximum 5 elements in a row.
					printf("\n");
			}
			free(arr);
			printf("\nThe sum of the sequence elements is: %lf\n\n", sum);
		}
	} while (n > 0);
	printf("\nThank you for exploring arithmetic sequences and dynamic allocations of arrays.\n");

	return 0;
}
 /* Function gets 3 pointers and ask user for filling them,
    and allocating a dynamic sized array by user input. */
double* inputArithmetic(double* a1, double* d, int* n)
{
	double* arr;
	printf("Enter a1, d and n respectively, please: ");
	scanf("%lf %lf %d", a1, d, n);
	if (*n <= 0) // if doesn't possible to make an arithmetic sequence.
		return NULL; // logical else
	arr = (double*)malloc((*n) * sizeof(double)); // allocating arr[*n].
	if (arr == NULL) { // if allocation faild.
		printf("AllocationError: sorry you're computer could not allocate memory\n");
		exit(1);
	}
	return arr;
}
/* Function that gets all parameters needed in order to
   fill an array with the arithmetic sequence elements. */
double setArithmetic(double a1, double d, double *arr, int n)
{
	int i;
	double sum = 0;
	for (i = 0; i < n; i++) {
		arr[i] = a1 + d * i; // calculating the element[i] in sequence.
		sum += arr[i];
	}
	return sum;
}

