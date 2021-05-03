#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>

void set_2d(float** a, int m, int n);
void print_2d(float** a, int m, int n);
void freeAllChildren(float**, int);

int main()
{
	int m, n, i;
	float** a;
	printf("\nenter m and n, for m*n array: \n");
	scanf("%d %d", &m, &n);

	a = (float**)malloc(m * sizeof(float*));
	if (a == NULL) {
		printf("AllocationError: system couldn't allocte memory!");
		return 1;
	}
	
	for (i = 0; i < m; i++) {
		a[i] = (float*)malloc(n * sizeof(float));
		if (a[i] == NULL) {
			printf("AllocationError: system couldn't allocte memory!");
			freeAllChildren(a, i);
			free(a);
			return 1;
		}
	}
	set_2d(a, m, n);
	print_2d(a, m, n);

	return 0;
}

void set_2d(float** a, int m, int n) 
{
	int i, j, k = 1;
	for (i = 0; i < m; i++)
		for (j = 0; j < n; j++)
			a[i][j] = k++;
}


void print_2d(float** a, int m, int n)
{
	int i, j;
	for (i = 0; i < m; i++) {
		for (j = 0; j < n; j++)
			printf("%8.1f", a[i][j]);
		printf("\n");
	}
}

void freeAllChildren(float **doublePtr, int allocatedSize)
{
	int i;
	for (i = 0; i < allocatedSize; i++)
		free(doublePtr[i]);
}