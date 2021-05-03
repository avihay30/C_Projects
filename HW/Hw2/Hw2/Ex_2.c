#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

typedef struct fraction {
	int numerator;
	int denominator;
} fraction;

int getFractionsInput(fraction[]);
void insertFractionToArr(fraction**, fraction*, int);
void simplify(fraction[], int, fraction*);
int gcd(int, int);
void printFractions(fraction[], int);
void printSimplied(fraction*);

int main()
{
	int arrSize;
	fraction* userFractions, simplifiedFrac;
	userFractions = (fraction*)malloc(sizeof(fraction)); // empty array with initial size of 1
	if (userFractions == NULL) {
		printf("AllocationError: system couldn't allocate memory!");
		return 1;
	}
	printf("Enter fractions by pairs of integers(0 0 to finish):\n");
	arrSize = getFractionsInput(userFractions);

	printFractions(userFractions, arrSize);
	simplify(userFractions, arrSize, &simplifiedFrac);
	printSimplied(&simplifiedFrac);

	free(userFractions);
	return 0;
}

int getFractionsInput(fraction fractionArr[])
{
	int arrSize = 0;
	fraction frac;
	while (1) {
		if (arrSize > 0 && frac.denominator == 0) {
			printf("\nYou can't divide by 0, please insert again...\n");
			arrSize--;
		}
		else if (arrSize > 0) { // iserting last inputted fraction.
			insertFractionToArr(&fractionArr, &frac, arrSize - 1);
		}
		printf("%d (mone mehane): ", arrSize + 1);
		scanf("%d %d", &frac.numerator, &frac.denominator);
		if (frac.numerator == 0 && frac.denominator == 0)
			break;
		arrSize++;
	}
	return arrSize;
}

void insertFractionToArr(fraction **fractionArr, fraction *frac, int index)
{
	fraction *tempP;
	if (index > 0) { // need to allocate more memory.
		tempP = (fraction*)realloc(*fractionArr, (index + 1) * sizeof(fraction));
		if (tempP == NULL) {
			free(*fractionArr);
			printf("AllocationError: system couldn't allocate memory!");
			exit(1);
		}
		*fractionArr = tempP;
	}
	(*fractionArr)[index] = *frac;
}

void simplify(fraction fractionArr[], int size, fraction *simlifiedFrac)
{
	int i, greatDivder = 1, gcdOfFrac;
	for (i = 0; i < size; i++)
		greatDivder *= fractionArr[i].denominator;
	for (i = 0; i < size; i++) {
		fractionArr[i].numerator *= greatDivder / fractionArr[i].denominator;
		fractionArr[i].denominator = greatDivder;
	}
	simlifiedFrac->denominator = greatDivder;
	simlifiedFrac->numerator = 0; // reset
	for (i = 0; i < size; i++) // adding all numerators to one.
		simlifiedFrac->numerator += fractionArr[i].numerator;

	gcdOfFrac = gcd(simlifiedFrac->numerator, simlifiedFrac->denominator);
	simlifiedFrac->numerator /= gcdOfFrac;
	simlifiedFrac->denominator /= gcdOfFrac;
}

int gcd(int a, int b) {
	return (b == 0) ? a : gcd(b, a % b);
}

void printFractions(fraction fractionArr[], int size)
{
	int i;
	for (i = 0; i < size; i++) {
		printf("%d/%d", fractionArr[i].numerator, fractionArr[i].denominator);
		if (i < size - 1) // not last fraction in arr
			printf(" + ");
	}
}

void printSimplied(fraction *simplfiedFrac)
{
	int intNum, moduleNum;
	printf(" = %d/%d = ", simplfiedFrac->numerator, simplfiedFrac->denominator);
	intNum = simplfiedFrac->numerator / simplfiedFrac->denominator;
	moduleNum = simplfiedFrac->numerator % simplfiedFrac->denominator;
	if (intNum != 0)
		printf("%d", intNum);
	if (moduleNum != 0)
		printf("%s%d/%d", (intNum == 0) ? "" : " and ", moduleNum, simplfiedFrac->denominator);
}