#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define SIZE_OF_CODE 10

typedef struct department {
	char code[SIZE_OF_CODE + 1];
	char *name;
	int soldItems;
} department;

typedef enum { FALSE, TRUE } Bool;

void inputToDepartmentArr(department[], int);
Bool isStrInputOk(char*, int);
void freeAllNames(department[], int);
Bool isPositiveInt(float);
void setPtrArrToBestWorstFirst(department* []);
void switchAddress(department**, department**);
void printDepartment(department* [], int, char*);

#define N 3
#define SIZE_OF_NAME 20
#define STR_TEMP_SIZE 256

int main()
{
	department departmentArr[N], *depPtrArr[N];
	int i;

	inputToDepartmentArr(departmentArr, N);
	for (i = 0; i < N; i++) // setting the array of pointers
		depPtrArr[i] = &departmentArr[i];
	
	setPtrArrToBestWorstFirst(depPtrArr);
	printf("|\\/\\/> RESULTS <\\/\\/|");
	printDepartment(depPtrArr, 0, "Best");
	printDepartment(depPtrArr, 1, "Worst");

	freeAllNames(departmentArr, N);
	return 0;
}

void inputToDepartmentArr(department depArr[], int size)
{
	int i;
	float dummyNum;
	char tempStr[STR_TEMP_SIZE];
	for (i = 0; i < size; i++) {
		printf("Enter department %d:", i + 1);
		do {
			printf("\n\tEnter code: ");
			gets(tempStr);
		} while (!isStrInputOk(tempStr, SIZE_OF_CODE));
		strcpy(depArr[i].code, tempStr);

		do {
			printf("\tEnter name: ");
			gets(tempStr);
		} while (!isStrInputOk(tempStr, SIZE_OF_NAME));
		depArr[i].name = (char*)malloc(strlen(tempStr) + 1); // sizeOf(char) = 1
		if (depArr[i].name == NULL) {
			printf("AllocationError: system couldn't allocate memory!");
			freeAllNames(depArr, i);
			exit(1);
		}
		strcpy(depArr[i].name, tempStr);

		while (TRUE) {
			printf("\tEnter number of products sold today: ");
			if (scanf("%f", &dummyNum) && isPositiveInt(dummyNum)) { // if equal to 0
				depArr[i].soldItems = (int)dummyNum;
				break;
			}
			printf("\tNot valid input, please enter a positive int number!\n");
			printf("\t``````````````````````````````````````````\n");
			rewind(stdin);
		}
		rewind(stdin);
		puts("____________________");
	}
}

int isStrInputOk(char *str, int maxSizeNeeded)
{
	if (strlen(str) > maxSizeNeeded) {
		printf("\tToo long input max size required is: %d!\n", maxSizeNeeded);
		printf("\t``````````````````````````````````````````\n");
		return FALSE;
	}
	return TRUE;
}

void freeAllNames(department depArr[], int size)
{
	int i; 
	for (i = 0; i < size; i++)
		free(depArr[i].name);
}

Bool isPositiveInt(float fNum)
{
	// decimal points equal to zero and number isn't negative.
	if (floor(fNum) == ceil(fNum) && fNum >= 0)
		return TRUE;
	return FALSE;
}

void setPtrArrToBestWorstFirst(department *depPtrArr[])
{
	int i;
	for (i = 1; i < N; i++) {
		if (depPtrArr[0]->soldItems < depPtrArr[i]->soldItems)
			switchAddress(&depPtrArr[0], &depPtrArr[i]);
		if (i > 1 && depPtrArr[1]->soldItems > depPtrArr[i]->soldItems)
			switchAddress(&depPtrArr[1], &depPtrArr[i]);
	}
}

void switchAddress(department** address1, department** address2)
{
	department* tempAddress;
	tempAddress = *address1;
	*address1 = *address2;
	*address2 = tempAddress;
}

void printDepartment(department* depPtrArr[], int index, char* title)
{
	printf("\n%s Department is:\n", title);
	printf("\tCode of department: %s\n", depPtrArr[index]->code);
	printf("\tName of department: %s\n", depPtrArr[index]->name);
}