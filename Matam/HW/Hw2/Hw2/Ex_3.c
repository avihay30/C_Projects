#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define SIZE_OF_CODE 9

typedef struct department {
	char code[SIZE_OF_CODE + 1]; // size = 9 + 1(includes '\0')
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
#define SIZE_OF_NAME 19 // size = 19 [without '\0']
#define STR_TEMP_SIZE 256 // exaggerated size of string for testing.

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

/* Function gets a fixed size array of departments and it's size,
   and fills the array with user inputs departments. */
void inputToDepartmentArr(department depArr[], int size)
{
	int i;
	float dummyNum; // for testing user input.
	char tempStr[STR_TEMP_SIZE]; // for testing user input.
	for (i = 0; i < size; i++) {
		printf("Enter department %d:", i + 1);
		do { // getting user input for code
			printf("\n\tEnter code: ");
			gets(tempStr);
		} while (!isStrInputOk(tempStr, SIZE_OF_CODE)); // checking if input longer than required
		strcpy(depArr[i].code, tempStr);

		do { // getting user input for name
			printf("\tEnter name: ");
			gets(tempStr);
		} while (!isStrInputOk(tempStr, SIZE_OF_NAME)); // checking if input longer than required
		depArr[i].name = (char*)malloc(strlen(tempStr) + 1); // sizeOf(char) = 1, size + 1 for '\0'
		if (depArr[i].name == NULL) {
			printf("AllocationError: system couldn't allocate memory!");
			freeAllNames(depArr, i);
			exit(1);
		}
		strcpy(depArr[i].name, tempStr);

		while (TRUE) { // getting user input for soldItems
			printf("\tEnter number of products sold today: ");
			// checking if scanf == 0 and the inputted num is positive. 
			if (scanf("%f", &dummyNum) && isPositiveInt(dummyNum)) {
				depArr[i].soldItems = (int)dummyNum;
				break;
			}
			printf("\tInvalid input, please enter a positive int number!\n");
			printf("\t``````````````````````````````````````````\n");
			rewind(stdin); // in case user inputs string.
		}
		rewind(stdin);
		puts("____________________");
	}
}


/* Function checks wether the string is in required length.
   we assume an empty string is INVALID input. */
int isStrInputOk(char *str, int maxSizeNeeded)
{
	char* mesg;
	if (strlen(str) > maxSizeNeeded || strlen(str) == 0) {
		mesg = (strlen(str) == 0) ? "Empty" : "Too long";
		printf("\t%s input: max size required is: %d!\n", mesg ,maxSizeNeeded);
		printf("\t`````````````````````````````````````````\n");
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

/* Function gets a float number and return wether that number is positive or not. */
Bool isPositiveInt(float fNum)
{
	// checking decimal points equal to zero and number isn't negative.
	if (floor(fNum) == ceil(fNum) && fNum >= 0)
		return TRUE;
	return FALSE; // logical else
}

/* Function gets an array of pointers
   and set the first to point to the largest and second to point the smallest
   (according to number of soldItems). */
void setPtrArrToBestWorstFirst(department *depPtrArr[])
{
	int i;
	for (i = 1; i < N; i++) { // iterating over the pointers.
		if (depPtrArr[0]->soldItems < depPtrArr[i]->soldItems)
			switchAddress(&depPtrArr[0], &depPtrArr[i]);
		if (i > 1 && depPtrArr[1]->soldItems > depPtrArr[i]->soldItems)
			switchAddress(&depPtrArr[1], &depPtrArr[i]);
	}
}

/* Function gets two address of pointers and assign one to point the other. */
void switchAddress(department** address1, department** address2)
{
	department* tempAddress;
	tempAddress = *address1;
	*address1 = *address2;
	*address2 = tempAddress;
}

/* Function gets array of pointers that points to departments,
   and an index and prints the parameters of the department with title. */
void printDepartment(department* depPtrArr[], int index, char* title)
{
	printf("\n%s Department is:\n", title);
	printf("\tCode of department: %s\n", depPtrArr[index]->code);
	printf("\tName of department: %s\n", depPtrArr[index]->name);
}