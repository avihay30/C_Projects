#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int InputThree(int* p1, int* p2, int* p3);
void SortTwo(int* p1, int* p2);
void SortThree(int* q1, int* q2, int* q3);

int main()
{
	int num1, num2, num3;
	/* Running in loop and calling InputThree function */
	while (InputThree(&num1, &num2, &num3) == 1)
	{
		/* Sorting all three numbers by reference, num1 is the smallest */
		SortThree(&num1, &num2, &num3);
		/* printing sorted values */
		printf("Sorted values: %d %d %d\n", num1, num2, num3);
	}
	printf("FINISH");
	return 0;
}

/* InputThree function gets 3 pointers 
	and assign them with the recived values from user */
int InputThree(int* p1, int* p2, int* p3)
{
	int ints_recived;
	printf("Input three integer values: ");
	ints_recived = scanf("%d %d %d", p1, p2, p3);
	if (ints_recived == 3)
		return 1;
	return 0; /* logical else */
}

/* SortTwo function gets 2 pointers
	and assign min(p1, p2) to p1 and max to p2 */
void SortTwo(int* p1, int* p2) 
{
	int temp;
	if (*p1 > *p2) {
		temp = *p1;
		*p1 = *p2;
		*p2 = temp;
	}
}

/* SortThree function gets 3 pointers
	and assign smallest to q1 and biggest to q3 */
void SortThree(int* q1, int* q2, int* q3)
{
	/* calling 3 times to SortTwo, each time with different pointers */
	SortTwo(q1, q2);
	SortTwo(q1, q3);
	SortTwo(q2, q3);
}
