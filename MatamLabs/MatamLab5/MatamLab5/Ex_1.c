#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

float avg(float[], int);

#define NUM_OF_RUNNERS 10

int main() 
{
	int i;
	float timeArr[NUM_OF_RUNNERS];
	printf("Enter times, please: ");
	for (i = 0; i < NUM_OF_RUNNERS; i++)
		scanf("%f", &timeArr[i]);

	float avgOfArr = avg(timeArr, NUM_OF_RUNNERS);
	int belowAvgRunners = 0;
	for (i = 0; i < NUM_OF_RUNNERS; i++)
		if (timeArr[i] < avgOfArr)
			belowAvgRunners++;

	printf("The number of runners, running below the average time is %d.\n", belowAvgRunners);
	return 0;
}

float avg(float arr[], int size)
{
	float sum = 0;
	int i;
	for (i = 0; i < size; i++)
		sum += arr[i];
	
	return sum / size;
}