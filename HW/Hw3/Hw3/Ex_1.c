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

void printUserMenu();
void getChoice(int*);
void uploadData(FILE**, university*);
void checkFile(FILE*);
Bool isStudentValid(student*, char*, char*);
void freeAllChildren(university*);
void setHwGrade(student*);
FILE* appendOutputFile(FILE**, int);
double getAvgGrades(university*);
void outputData(FILE*, university*);
void printStudentInfo(FILE*, student*, int);
void outputFinalGrades(FILE*, university*);
void outputStatistics(FILE*, university*);
void outputAboveAvgStudents(FILE*, university*);

int main()
{
	Bool isFirstRun = TRUE;
	int choice = 0;
	FILE* f;
	university uni;

	printf("Welcome User,\n");
	while (choice != 6) {
		if (isFirstRun) {
			printf("\t**** On first run, choose option NO.1 ****\n");
			f = fopen("output.txt", "wt"); // only creating new file.
			checkFile(f);
			fclose(f);
		}
		printUserMenu();
		getChoice(&choice);
		if (choice != 1 && isFirstRun) { // todo: check if 6 is valid at start..
			do {
				printf("\tInvalid input! on first run choose option NO.1\n");
				getChoice(&choice);
			} while (choice != 1);
		}
		if (choice == 1 && !isFirstRun) {
			do {
				printf("\tInvalid input! You cannot choose option 1 twice.\n");
				getChoice(&choice);
			} while (choice == 1);
		}
		if (choice == 1 && isFirstRun) {
			uploadData(&f, &uni);
			isFirstRun = FALSE;
			continue;
		}
		f = appendOutputFile(&f, choice);
		switch (choice) {
		/*case 1:
			uploadData(&f, &uni);
			isFirstRun = FALSE;
			break; */
		case 2:
			//f = appendOutputFile(&f, choice);
			outputData(f, &uni);
			//fclose(f);
			break;
		case 3:
			//f = appendOutputFile(&f, choice);
			outputFinalGrades(f, &uni);
			//fclose(f);
			break;
		case 4:
			//f = appendOutputFile(&f, choice);
			outputStatistics(f, &uni);
			//fclose(f);
			break;
		case 5:
			//f = appendOutputFile(&f, choice);
			outputAboveAvgStudents(f, &uni);
			//fclose(f);
			break;
		case 6:
			//f = appendOutputFile(&f, choice);
			fprintf(f, "End Of Program");
			//fclose(f);
			break;
		}
		fclose(f);
	}
	printf("Goodbay!\n");
	freeAllChildren(&uni);
	free(uni.students);
	return 0;
}

void printUserMenu()
{
	printf("\tPlease choose an action:\n");
	printf("\t-> 1: Upload data from 'input.txt' file.\n");
	printf("\t-> 2: Output student's data.\n");
	printf("\t-> 3: Calculate final grades.\n");
	printf("\t-> 4: Calculate statisticts data.\n");
	printf("\t-> 5: Output only above average students.\n");
	printf("\t-> 6: End program.\n");
	printf("\t(*NOTE: 2<->5 will be output into 'output.txt' file)\n");
	printf("\t```````````````````````````````````````````````````````\n");
}

void getChoice(int *choice)
{
	printf("\tYour choice: ");
	while (TRUE) {
		if (scanf("%d", choice) && (*choice > 0 && *choice < 7))
			break;
		printf("\tInvalid input! please enter int between 1<->6: "); // logical else
		rewind(stdin); // in case user inputs string.
	}
	puts("\n");
}

void uploadData(FILE** inputFile, university* uni)
{
	int i = 0, numOfInputs = 0;
	char tempName[100], tempGrades[50];
	student tmpStudent, *tempStudents;

	*inputFile = fopen("input.txt", "rt");
	checkFile(*inputFile);

	while ((numOfInputs = fscanf(*inputFile, "%s %d %f %s", tempName, &tmpStudent.id, &tmpStudent.grade, &tempGrades)) != EOF) {
		if (numOfInputs != 4 || !isStudentValid(&tmpStudent, tempName, tempGrades)) {
			printf("Read-File Error: the input file isn't valid!!\n");
			if (i > 0) {
				freeAllChildren(uni);
				free(uni->students);
			}
			exit(1);
		}

		if (i == 0) {
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
				freeAllChildren(uni);
				free(uni->students);
				exit(1);
			}
			uni->students = tempStudents;
		}
		uni->students[i].name = (char*)malloc(strlen(tempName) + 1); // sizeOf(char) = 1
		if (uni->students[i].name == NULL) {
			printf("%s\n", ALLOC_ERR);
			if (i > 0) // already done malloc for names.
				freeAllChildren(uni);
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
	fclose(*inputFile);
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
		exit(1); // closes file automaticly
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

void freeAllChildren(university* uni)
{
	int i;
	for (i = 0; i < uni->numOfStudents; i++)
		free(uni->students[i].name);
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

FILE* appendOutputFile(FILE** outputFile, int option)
{
	*outputFile = fopen("output.txt", "at");
	checkFile(*outputFile);
	fprintf(*outputFile, "Option %d:\n", option);
	fprintf(*outputFile, "``````````\n");
	return *outputFile;
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
	student *stud; // for readablity

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
	student* stud; // for readablity
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
	student* stud = uni->students; // for readablity
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