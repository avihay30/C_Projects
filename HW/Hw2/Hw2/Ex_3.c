#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE_OF_CODE 10

typedef struct department {
	char code[SIZE_OF_CODE + 1];
	char *name;
	int soldItems;
} department;

void inputToDepartmentArr(department[], int);
int isStrInputOk(char*, int);
void freeAllNames(department[], int);

#define N 10
#define SIZE_OF_NAME 20
#define STR_TEMP_SIZE 256
#define TRUE 1
#define FALSE 0

int main()
{
	department departmentArr[N];
	inputToDepartmentArr(departmentArr, N);

	return 0;
}

void inputToDepartmentArr(department depArr[], int size)
{
	int i;
	char tempStr[STR_TEMP_SIZE];
	for (i = 0; i < size; i++) {
		printf("Enter department %d:", i + 1);
		do {
			printf("\n\tEnter code: ");
			fgets(tempStr, STR_TEMP_SIZE, stdin);
		} while (!isStrInputOk(tempStr, SIZE_OF_CODE));
		strcpy(depArr[i].code, tempStr);

		do {
			printf("\tEnter name: ");
			fgets(tempStr, STR_TEMP_SIZE, stdin);
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
			if (scanf("%d", &depArr[i].soldItems)) // if equal to 0
				break;
			printf("\tNot valid input, please enter a number!\n");
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