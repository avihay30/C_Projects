#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>

#define N 5
#define M 4
#define TRUE 1 // declaring two variables of true/false for convenience.
#define FALSE 0

double average(int Matrix[][M], int Rows, int Cols, int r, int c);
int isValidIndex(int, int, int, int);

int main()
{
	int mat[N][M] = {{11, 12, 13, 14},
					{0, -7, 18, 7},
					{1, 2, -1, -2},
					{6, -9, -19, 9},
					{300, 149, 267, 10}}; // declaring an hard-coded matrix.
	double avg;
	int row, col;
	printf("Enter the row and col respectively of the element: ");
	scanf("%d %d", &row, &col);
	avg = average(mat, N, M, row, col);
	printf("The average is: %.2lf\n", avg); // %.2lf showing only 2 decimal numbers.
	return 0;
}

/* Function gets a double pointer(a matrix) and it's size 
   and a specific index in that matrix   */
double average(int Matrix[][M], int Rows, int Cols, int r, int c)
{
	int i, j, sum = 0, counter = 0;
	if (!isValidIndex(Rows, Cols, r, c)) { // checking if Matrix[r][c] exists.
		printf("IndexOutOfBoundsException: the index you're looking for doesn't exists in the matrix.\n");
		exit(1);
	}

	// iterating over neighbors of a specific element in the matrix 
	for (i = r - 1; i < r + 2; i++) {
		for (j = c - 1; j < c + 2; j++) {
			// not taking in count the element itself Matrix[r][c] and non existing elements.
			if ((i != r || j != c) && isValidIndex(Rows, Cols, i, j)) {
				sum += Matrix[i][j];
				counter++;
			}
		}
	}
	return (double)sum / counter;
}

int isValidIndex(int rows, int cols, int r, int c)
{
	// checking if element inside the bounds of the matrix.
	if (r >= 0 && r < rows && c >= 0 && c < cols)
		return TRUE;
	return FALSE;
}
