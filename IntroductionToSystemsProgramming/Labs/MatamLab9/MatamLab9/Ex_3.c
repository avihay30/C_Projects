#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define N 4

typedef struct Complex
{
	float real;
	float im;
} Complex;

double CRadius(Complex*);
void Error_Msg(char* str);
void InputAndWriteToFile(FILE* f);
int CheckFile(FILE* f, float m);

int main()
{
	FILE* f;
	if ((f = fopen("complex_number.txt", "wt")) == NULL)
		Error_Msg("The input file is wrong");

	InputAndWriteToFile(f);
	fclose(f);

	if ((f = fopen("complex_number.txt", "rt")) == NULL)
		Error_Msg("The input file is wrong");
	printf("\nThere are %d big numbers\n", CheckFile(f, 4));
	fclose(f);
	return 0;
}

/* The function gets a pointer to a complex number and
   retruns the absolute radius. */
double CRadius(Complex* complex)
{
	return sqrt(pow(complex->real, 2) + pow(complex->im, 2));
}

void Error_Msg(char* str)
{
	printf("\n%s", str);
	exit(1);
}

void InputAndWriteToFile(FILE* f)
{
	int i;
	Complex tempComp;
	printf("Enter %d complex numbers\n", N);
	for (i = 0; i < N; i++) {
		if (scanf("%f %f", &tempComp.real, &tempComp.im) == 2)
			fprintf(f, "%.1f %.1f %.1f\n", tempComp.real, tempComp.im, CRadius(&tempComp));
	}
}

int CheckFile(FILE* f, float m)
{
	int i, count = 0;
	float num = 0;

	for (i = 0; i < N; i++) {
		fscanf(f, "%f %f %f", &num, &num, &num);
		if (num > m)
			count++;
	}
	return count;
}