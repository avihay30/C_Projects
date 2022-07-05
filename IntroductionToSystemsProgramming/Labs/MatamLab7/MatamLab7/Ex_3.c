#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<math.h>

typedef struct Complex
{
	float real;
	float im;
} Complex;

double CRadius(Complex*);
Complex* CMax(Complex[], int);

#define NUM 3

int main()
{
	int i;
	Complex complexArr[NUM], *maxComp;
	printf("Enter %d complex numbers:\n", NUM);

	for (i = 0; i < NUM; i++) { // filling the array
		scanf("%f %f", &complexArr[i].real, &complexArr[i].im);
	}
	maxComp = CMax(complexArr, NUM);
	printf("\nThe max complex number is: %.2f+%.2fi", maxComp->real, maxComp->im);
	printf("\nThe radius of the max number is: %.2lf\n", CRadius(maxComp));
}

/* The function gets a pointer to a complex number and 
   retruns the absolute radius. */
double CRadius(Complex* complex)
{
	return sqrt(pow(complex->real, 2) + pow(complex->im, 2));
}

/* The function gets a pointer to an array of complex numbers and
   retruns a pointer to the complex number with max absolute radius. */
Complex* CMax(Complex complexArr[], int size)
{
	int i, indexOfMax;
	double max = 0, temp;
	for (i = 0; i < size; i++) { // iterating over the array.
		temp = CRadius(&complexArr[i]);
		if (temp > max) {
			max = temp;
			indexOfMax = i;
		}
	}
	return &complexArr[indexOfMax]; // returning pointer of max complex.
}
