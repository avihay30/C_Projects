#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

float avg(float[], int);

#define NUM_OF_RUNNERS 10

int main() 
{
	int i, belowAvgRunners;
	float timeArr[NUM_OF_RUNNERS], avgOfArr;
	printf("Enter times, please: ");
	// getting inputs for every runner and inserting to the array.
	for (i = 0; i < NUM_OF_RUNNERS; i++)
		scanf("%f", &timeArr[i]);

	avgOfArr = avg(timeArr, NUM_OF_RUNNERS);
	belowAvgRunners = 0;
	for (i = 0; i < NUM_OF_RUNNERS; i++) // itereting over the array.
		if (timeArr[i] < avgOfArr) // checking if runner lower than avg.
			belowAvgRunners++;

	printf("The number of runners, running below the average time is %d.\n", belowAvgRunners);
	return 0;
}

/* Function gets a pointer to array and it's size,
   and returns the average of all elements in the array. */
float avg(float arr[], int size)
{
	float sum = 0;
	int i;
	for (i = 0; i < size; i++)
		sum += arr[i];
	
	return sum / size;
}