#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MARKS_LEN 4

typedef struct stud
{
	char* name;
	int marks[MARKS_LEN];
	float avg;
} student;


student* Create_Class(int);
void inputName(student*, int);
void inputMarks(student*);
void Avg_Mark(student*);
void Print_One(student*);

#define MAX_NAME_LEN 50

int main()
{
	int size, i;
	student* arr;
	printf("Enter the number of students: ");
	scanf("%d", &size);
	
	arr = Create_Class(size);
	for (i = 0; i < size; i++) {
		if (arr[i].avg > 85)
			Print_One(&arr[i]);
	}
	free(arr);
	return 0;
}

/* The function gets a size and 
   retruns an array of students accorrding to user input */
student* Create_Class(int size)
{
	int i;
	student* studentArr;
	studentArr = (student*)malloc(sizeof(student) * size);
	if (studentArr == NULL) { // if malloc failed.
		printf("\nAllocationError: malloc function could not malloc memory.");
		exit(1);
	}
	for (i = 0; i < size; i++) { // iterating over the studentArr
		inputName(studentArr, i);
		inputMarks(&studentArr[i]);
		Avg_Mark(&studentArr[i]);
	}
	printf("\n");
	return studentArr;
}

/* The function gets a studentArray and index of a specific student
   and assing to that student the name according to user input. */
void inputName(student* s, int i) 
{
	char str[MAX_NAME_LEN];
	printf("\nEnter your name: ");
	scanf("%s", str);
	s[i].name = (char*)malloc(strlen(str) + 1);
	if (s[i].name == NULL) { // if malloc failed.
		printf("\nAllocationError: malloc function could not malloc memory.");
		free(s);
		exit(1);
	}
	strcpy(s[i].name, str);
}

/* The function gets a pointer to a specific student
   and assing to that student marks according to user input. */
void inputMarks(student* s)
{
	int i;
	printf("\nEnter your marks: ");
	for (i = 0; i < MARKS_LEN; i++)
		scanf("%d", &(s -> marks[i]));
}

/* The function gets a pointer of a specific student
   and calculating the avg and assigning the value to the struc. */
void Avg_Mark(student* s)
{
	int i, sum = 0;
	for (i = 0; i < MARKS_LEN; i++) {
		sum += s->marks[i];
	}
	s->avg = (float)sum / MARKS_LEN;
}


void Print_One(student* s)
{
	printf("The average of %s is %.1f\n", s->name, s->avg);
}

