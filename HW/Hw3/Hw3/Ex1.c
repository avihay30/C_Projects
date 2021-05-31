#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define NUM_OF_GRADES 5
#define ALLOC_ERR "AlloctionError: The program couldn't allocate memory!"
#define FILE_ERR "FileError: The program couldn't create/read file!"
#define FILE_READ_ERR "Read-File Error: the input file is empty or invalid!!"

typedef enum { FALSE, TRUE } Bool;

typedef struct student {
	char* name;
	long id;
	float grade;
	char hwGrades[NUM_OF_GRADES + 1]; // +1 for '\0'.
	char finalHwGrade;
} student;

typedef struct university {
	student* students;
	int numOfStudents;
} university;

void printUserMenu(Bool);
void getChoice(double*, int*);
void checkAllocation(void*, char*, university*);
void uploadData(FILE*, university*);
Bool isStudentValid(student*, char*, char*);
void freeAll(university*);
void setHwGrade(student*);
void outputData(FILE*, university*);
void outputFinalGrades(FILE*, university*);
double getAvgGrades(university*);
void outputStatistics(FILE*, university*);
void outputAboveAvgStudents(FILE*, university*);

int main()
{
	Bool isFirstRun = TRUE;
	double dummyChoice = 0; // for tests in case user enters non int value.
	int choice = 0; // will hold the real choice.
	FILE* f = NULL;
	university uni;
	uni.students = NULL; // setting values of uni to default values.
	uni.numOfStudents = 0;

	printf("Welcome User,\n");
	while (choice != 6) {
		printUserMenu(isFirstRun); // prints menu.
		getChoice(&dummyChoice, &choice); // get choice and checks it's validation.
		if (choice != 1 && isFirstRun) {
			do {
				printf("\tInvalid input! on first run choose option NO.1\n");
				getChoice(&dummyChoice, &choice); // ask user for new valid input.
			} while (choice != 1);
		}
		if (choice == 1 && !isFirstRun) {
			do {
				printf("\tInvalid input! You cannot choose option NO.1 *twice*.\n");
				getChoice(&dummyChoice, &choice); // ask user for new valid input.
			} while (choice == 1);
		}
		if (choice != 1) // choice is in [2-5]
			fprintf(f, "Option %d:\n``````````\n", choice);
		switch (choice) {
		case 1: // enters only if (choice == 1 and isFirstRun)
			f = fopen("input.txt", "rt");
			checkAllocation(f, FILE_ERR, &uni);
			uploadData(f, &uni);
			fclose(f);
			isFirstRun = FALSE;
			f = fopen("output.txt", "wt"); // creating new file for future outputs.
			checkAllocation(f, FILE_ERR, &uni);
			break;
		case 2:
			outputData(f, &uni);
			break;
		case 3:
			outputFinalGrades(f, &uni);
			break;
		case 4:
			outputStatistics(f, &uni);
			break;
		case 5:
			outputAboveAvgStudents(f, &uni);
			break;
		case 6:
			fprintf(f, "End Of Program");
			break;
		}
	}
	printf("^^^\\> Goodbay! </^^^\n");
	printf("`````````````````````\n");
	fclose(f);
	freeAll(&uni);
	PrintMemoryLeaks();
	return 0;
}

// Function prints all availble user inputs.
void printUserMenu(Bool isFirstRun)
{
	if (isFirstRun)
		printf("\t**** On first run, choose option NO.1 ****\n");
	printf("\tPlease choose an action:\n");
	printf("\t-> 1: %sUpload data from 'input.txt' file.\n", !isFirstRun ? "[DISABLED] " : "");
	printf("\t-> 2: Output student's data.\n");
	printf("\t-> 3: Calculate final grades.\n");
	printf("\t-> 4: Calculate statistics data.\n");
	printf("\t-> 5: Output only above average students.\n");
	printf("\t-> 6: End program.\n");
	printf("\t(*NOTE: 2<->5 will be output into 'output.txt' file)\n");
	printf("\t```````````````````````````````````````````````````````\n");
}

/* Function gets a dummyChoice(float pointer) and int pointer
   and cast the float pointer to int only if user inputted a valid input.*/
void getChoice(double *fchoice, int *choice)
{
	printf("\tYour choice: ");
	while (TRUE) {
		// getting input and checking if it's int(by floor and ceil) and it's boundaries[0,6].
		if (scanf("%lf", fchoice) && floor(*fchoice) == ceil(*fchoice) && (*fchoice > 0.0 && *fchoice < 7.0)) {
			*choice = (int)*fchoice; // user entered int (fchoice is an 'int', like 2.0).
			break;
		}
		printf("\tInvalid input! please enter *int* between 1<->6: "); // logical else
		rewind(stdin); // in case user inputs string.
	}
	puts("\n");
}

/* Function gets pointer to a file and an a pointer to an empty university,
   and reads the data from file and assing it to the university. */
void uploadData(FILE* inputFile, university* uni)
{
	int i = 0, numOfInputs = 0;
	char tempName[256], tempGrades[50]; // dummy strings for testing.
	student tmpStudent, *tempStudents; // dummy pointers for testing.

	// looping until file ended.
	while ((numOfInputs = fscanf(inputFile, "%s %d %f %s", tempName, &tmpStudent.id, &tmpStudent.grade, &tempGrades)) != EOF) {
		// checking if file is invalid
		if (numOfInputs != 4 || !isStudentValid(&tmpStudent, tempName, tempGrades))
			// passing to function NULL on perpose to free all memory if need and exit.
			checkAllocation(NULL, FILE_READ_ERR, uni);

		if (i == 0) { // doing malloc only if file isn't empty and valid.
			uni->students = (student*)malloc(sizeof(student));
			checkAllocation(uni->students, ALLOC_ERR, uni);
		} else {
			tempStudents = (student*)realloc(uni->students, (i + 1) * sizeof(student));
			checkAllocation(tempStudents, ALLOC_ERR, uni);
			uni->students = tempStudents;
		}
		uni->students[i].name = (char*)malloc(strlen(tempName) + 1); // sizeOf(char) = 1
		checkAllocation(uni->students[i].name, ALLOC_ERR, uni);

		// filling student data.
		strcpy(uni->students[i].name, tempName);
		uni->students[i].id = tmpStudent.id;
		uni->students[i].grade = tmpStudent.grade;
		strcpy(uni->students[i].hwGrades, tempGrades);
		setHwGrade(&uni->students[i]);
		uni->numOfStudents = ++i; // increment before assignment
	}
	if (i == 0) { // if whole file is empty.
		fprintf(stderr, "%s\n", FILE_READ_ERR);
		exit(1);
	}
}

// Function checks if pointer allocation is invalid and free memory needed and exit. 
void checkAllocation(void *pToCheck, char *message, university *uni)
{
	if (pToCheck == NULL) {
		fprintf(stderr, "%s\n", message);
		freeAll(uni);
		exit(1); // closes file automaticaly
	}
}
/* Function gets a temp student struct and dummy name and grades
   and checks whether the student is valid. */
Bool isStudentValid(student* stud, char* name, char* grades)
{
	int i;
	if (strlen(name) > 99 || strlen(grades) != NUM_OF_GRADES) // checking grades and name
		return FALSE;
	if (stud->id <= 9999 || stud->id > 99999)
		return FALSE;
	if (stud->grade < 0 || stud->grade > 100) // assume valid grade range is [0,100]
		return FALSE;
	for (i = 0; i < NUM_OF_GRADES; i++)
		if (grades[i] != '0' && grades[i] != '1') // grades not binary.
			return FALSE;

	return TRUE; // logical else
}

// Function gets an pointer and free all allocated memory.
void freeAll(university* uni)
{
	int i;
	if (uni->students == NULL)
		return;
	for (i = 0; i < uni->numOfStudents; i++)
		free(uni->students[i].name);
	free(uni->students);
}

/* Function gets a student and calculates 
   and assing the final grade of Hw to student. */
void setHwGrade(student* student)
{
	int i, cnt = 0;
	for (i = 0; i < NUM_OF_GRADES; i++) {
		// checking if grade == '1' -> increment before assignment cnt.
		if (student->hwGrades[i] == '1' && ++cnt == 3) {
			student->finalHwGrade = '1';
			return;
		}
	}
	student->finalHwGrade = '0';
}

/* Function gets a file pointer and filled university pointer 
   and write the university to the file */
void outputData(FILE* outputFile, university* uni)
{
	int i;
	student *stud = uni->students; // for readability

	for (i = 0; i < uni->numOfStudents; i++, stud++) { // increment pointer of stud.
		// printing student to file.
		fprintf(outputFile, "Student %d: %s", i + 1, stud->name);
		fprintf(outputFile, " %d %.2f %c", stud->id, stud->grade, stud->finalHwGrade);
		fputc('\n', outputFile);
	}
	fputc('\n', outputFile);
}

/* Function gets a file pointer and filled university pointer
   and write the university to the file and the final grades of each student. */
void outputFinalGrades(FILE* outputFile, university* uni)
{
	int i;
	double finalGrade;
	student* stud = uni->students; // for readability
	fprintf(outputFile, "BEFORE:\n");
	outputData(outputFile, uni);
	fprintf(outputFile, "AFTER:\n");
	for (i = 0; i < uni->numOfStudents; i++, stud++) { // increment pointer of stud.
		// calculating final grade.
		if (stud->grade < 55)
			finalGrade = stud->grade;
		else {
			finalGrade = stud->grade * 0.85;
			if (stud->finalHwGrade == '1')
				finalGrade += 15;
		}
		// printing student to file.
		fprintf(outputFile, "Student %d: %s", i + 1, stud->name);
		fprintf(outputFile, " %d %.2f %c", stud->id, stud->grade, stud->finalHwGrade);
		fprintf(outputFile, " final: %.2lf\n", finalGrade);
	}
	fputc('\n', outputFile);
}

/* Function gets a university pointer and calcs avg of students. */
double getAvgGrades(university* uni)
{
	int i;
	double avg = 0;
	for (i = 0; i < uni->numOfStudents; i++)
		avg += uni->students[i].grade;
	avg /= uni->numOfStudents;

	return avg;
}

/* Function gets a file pointer and filled university pointer
   and write the statistics of the students to the file */
void outputStatistics(FILE* outputFile, university* uni)
{
	int i;
	double avg, sd = 0;
	student* stud = uni->students; // for readability
	float minGrade = stud->grade, maxGrade = stud->grade;

	avg = getAvgGrades(uni);
	for (i = 0; i < uni->numOfStudents; i++, stud++) { // increment pointer of stud.
		sd += pow(stud->grade - avg, 2.0); // calc beneath sqrt in Standard Deviation
		minGrade = (minGrade > stud->grade) ? stud->grade : minGrade;
		maxGrade = (maxGrade < stud->grade) ? stud->grade : maxGrade;
	}
	sd = sqrt(sd / uni->numOfStudents);

	// printing results to file.
	fprintf(outputFile, "Average: %.2lf\n", avg);
	fprintf(outputFile, "Standard Deviation: %.2lf\n", sd);
	fprintf(outputFile, "Number of students: %d\n", uni->numOfStudents);
	fprintf(outputFile, "Range: %.2lf - %.2lf\n\n", maxGrade, minGrade);
}

/* Function gets a file pointer and filled university pointer
   and write the above average students[name, id] to the file */
void outputAboveAvgStudents(FILE* outputFile, university* uni)
{
	int i;
	double avg;
	avg = getAvgGrades(uni); // avg of all students.

	for (i = 0; i < uni->numOfStudents; i++)
		if (uni->students[i].grade > avg)
			fprintf(outputFile, "Name: %s, Id: %d\n", uni->students[i].name, uni->students[i].id);
	fputc('\n', outputFile);
}