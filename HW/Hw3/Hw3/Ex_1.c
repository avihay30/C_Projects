#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define NUM_OF_GRADES 5
#define ALLOC_ERR "AlloctionError: The program couldn't allocate memory!"
#define FILE_ERR "FileError: The program couldn't create/read file!"

typedef enum { FALSE, TRUE } Bool;

typedef struct student {
	char* name;
	long id;
	float grade;
	char hwGrades[NUM_OF_GRADES + 1]; // +1 for '\0'
	char finalHwGrade;
} student;

typedef struct university {
	student* students;
	int numOfStudents;
} university;

void printUserMenu(Bool);
void getChoice(double*, int*);
void checkFile(FILE*);
void uploadData(FILE*, university*);
Bool isStudentValid(student*, char*, char*);
void freeAll(university*);
void setHwGrade(student*);
double getAvgGrades(university*);
void outputData(FILE*, university*);
void printStudentInfo(FILE*, student*, int);
void outputFinalGrades(FILE*, university*);
void outputStatistics(FILE*, university*);
void outputAboveAvgStudents(FILE*, university*);

int main()
{
	Bool isFirstRun = TRUE;
	double dummyChoice = 0; // for tests in case user enters non int value.
	int choice = 0;
	FILE* f = NULL;
	university uni;

	printf("Welcome User,\n");
	while (choice != 6) {
		printUserMenu(isFirstRun);
		getChoice(&dummyChoice, &choice);
		if (choice != 1 && isFirstRun) {
			do {
				printf("\tInvalid input! on first run choose option NO.1\n");
				getChoice(&dummyChoice, &choice);
			} while (choice != 1);
		}
		if (choice == 1 && !isFirstRun) {
			do {
				printf("\tInvalid input! You cannot choose option NO.1 *twice*.\n");
				getChoice(&dummyChoice, &choice);
			} while (choice == 1);
		}
		if (choice != 1) // choice is in [2-5]
			fprintf(f, "Option %d:\n``````````\n", choice);
		switch (choice) {
		case 1: // enter only if (choice == 1 and isFirstRun)
			f = fopen("input.txt", "rt");
			checkFile(f);
			uploadData(f, &uni);
			fclose(f);
			isFirstRun = FALSE;
			f = fopen("output.txt", "wt"); // only creating new file for outputs.
			checkFile(f);
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
	return 0;
}

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

void getChoice(double *fchoice, int *choice)
{
	printf("\tYour choice: ");
	while (TRUE) {
		if (scanf("%lf", fchoice) && floor(*fchoice) == ceil(*fchoice) && (*fchoice > 0.0 && *fchoice < 7.0)) {
			*choice = (int)*fchoice; // fchoice is an 'int', like 2.0
			break;
		}
		printf("\tInvalid input! please enter *int* between 1<->6: "); // logical else
		rewind(stdin); // in case user inputs string.
	}
	puts("\n");
}

void uploadData(FILE* inputFile, university* uni)
{
	int i = 0, numOfInputs = 0;
	char tempName[100], tempGrades[50];
	student tmpStudent, *tempStudents;

	while ((numOfInputs = fscanf(inputFile, "%s %d %f %s", tempName, &tmpStudent.id, &tmpStudent.grade, &tempGrades)) != EOF) {
		if (numOfInputs != 4 || !isStudentValid(&tmpStudent, tempName, tempGrades)) {
			printf("Read-File Error: the input file isn't valid!!\n");
			if (i > 0)
				freeAll(uni);
			exit(1);
		}

		if (i == 0) { // doing malloc only if file isn't empty.
			uni->students = (student*)malloc(sizeof(student));
			if (uni->students == NULL) {
				printf("%s\n", ALLOC_ERR);
				exit(1);
			}
		}
		else {
			tempStudents = (student*)realloc(uni->students, (i + 1) * sizeof(student));
			if (tempStudents == NULL) {
				printf("%s\n", ALLOC_ERR);
				freeAll(uni);
				exit(1);
			}
			uni->students = tempStudents;
		}
		uni->students[i].name = (char*)malloc(strlen(tempName) + 1); // sizeOf(char) = 1
		if (uni->students[i].name == NULL) {
			printf("%s\n", ALLOC_ERR);
			if (i > 0) // already done malloc for names.
				freeAll(uni);
			else
				free(uni->students);
			exit(1);
		}

		strcpy(uni->students[i].name, tempName);
		uni->students[i].id = tmpStudent.id;
		uni->students[i].grade = tmpStudent.grade;
		strcpy(uni->students[i].hwGrades, tempGrades);
		setHwGrade(&uni->students[i]);
		i++;
	}
	if (i == 0) {
		printf("Read-File Error: The input file is empty!!\n");
		exit(1);
	}
	uni->numOfStudents = i;
}

void checkFile(FILE* f)
{
	if (f == NULL) {
		printf("%s\n", FILE_ERR);
		exit(1); // closes file automaticaly
	}
}

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

void freeAll(university* uni)
{
	int i;
	for (i = 0; i < uni->numOfStudents; i++)
		free(uni->students[i].name);
	free(uni->students);
}

void setHwGrade(student* student)
{
	int i, cnt = 0;
	for (i = 0; i < NUM_OF_GRADES; i++) {
		if (student->hwGrades[i] == '1' && ++cnt == 3) {
			student->finalHwGrade = '1';
			return;
		}
	}
	student->finalHwGrade = '0';
}

double getAvgGrades(university* uni)
{
	int i;
	double avg = 0;
	for (i = 0; i < uni->numOfStudents; i++)
		avg += uni->students[i].grade;
	avg /= uni->numOfStudents;

	return avg;
}

void outputData(FILE* outputFile, university* uni)
{
	int i;
	student *stud; // for readability

	for (i = 0; i < uni->numOfStudents; i++) {
		stud = &(uni->students[i]);
		printStudentInfo(outputFile, stud, i + 1);
		fputc('\n', outputFile);
	}
	fputc('\n', outputFile);
}

void printStudentInfo(FILE* outputFile, student* stud, int studNum)
{
	fprintf(outputFile, "Student %d: %s", studNum, stud->name);
	fprintf(outputFile, " %d %.2f %c", stud->id, stud->grade, stud->finalHwGrade);
}

void outputFinalGrades(FILE* outputFile, university* uni)
{
	int i;
	double finalGrade;
	student* stud; // for readability
	fprintf(outputFile, "BEFORE:\n");
	outputData(outputFile, uni);
	fprintf(outputFile, "AFTER:\n");
	for (i = 0; i < uni->numOfStudents; i++) {
		stud = &(uni->students[i]);
		if (stud->grade < 55)
			finalGrade = stud->grade;
		else {
			finalGrade = stud->grade * 0.85;
			if (stud->finalHwGrade == '1')
				finalGrade += 15;
		}
		printStudentInfo(outputFile, stud, i + 1);
		fprintf(outputFile, " final: %.2lf\n", finalGrade);
	}
	fputc('\n', outputFile);
}

void outputStatistics(FILE* outputFile, university* uni)
{
	int i;
	double avg, sd = 0;
	student* stud = uni->students; // for readability
	float minGrade = stud->grade, maxGrade = stud->grade;

	avg = getAvgGrades(uni);
	for (i = 0; i < uni->numOfStudents; i++) {
		stud = &(uni->students[i]);
		sd += pow(stud->grade - avg, 2.0); // calc Standard Deviation
		minGrade = (minGrade > stud->grade) ? stud->grade : minGrade;
		maxGrade = (maxGrade < stud->grade) ? stud->grade : maxGrade;
	}
	sd = sqrt(sd / uni->numOfStudents);

	fprintf(outputFile, "Average: %.2lf\n", avg);
	fprintf(outputFile, "Standard Deviation: %.2lf\n", sd);
	fprintf(outputFile, "Number of students: %d\n", uni->numOfStudents);
	fprintf(outputFile, "Range: %.2lf - %.2lf\n\n", maxGrade, minGrade);
}

void outputAboveAvgStudents(FILE* outputFile, university* uni)
{
	int i;
	double avg;
	avg = getAvgGrades(uni);

	for (i = 0; i < uni->numOfStudents; i++)
		if (uni->students[i].grade > avg)
			fprintf(outputFile, "Name: %s, Id: %d\n", uni->students[i].name, uni->students[i].id);
	fputc('\n', outputFile);
}