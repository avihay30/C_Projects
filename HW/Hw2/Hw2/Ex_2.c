#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

typedef struct fraction {
	int numerator; // mone
	int denominator; // mehane
} fraction;

int getFractionsInput(fraction*[]);
void insertFractionToArr(fraction**, fraction*, int);
void simplify(fraction[], int, fraction*);
int gcd(int, int);
void printFractions(fraction[], int);
void printSimplied(fraction*);

int main()
{
	fraction* userFractions, simplifiedFrac;
	int arrSize; // will hold the actual size after user input.
	userFractions = (fraction*)malloc(sizeof(fraction)); // empty array with initial size of 1
	if (userFractions == NULL) {
		printf("AllocationError: system couldn't allocate memory!");
		return 1;
	}
	printf("Enter fractions by pairs of integers(0 0 to finish):\n");
	arrSize = getFractionsInput(&userFractions);

	if (arrSize == 0) {
		printf("InputError: program ended without any calculation.\n");
		return 1;
	}
	printFractions(userFractions, arrSize); // prints user input fractions.
	simplify(userFractions, arrSize, &simplifiedFrac);
	printSimplied(&simplifiedFrac);

	free(userFractions);
	return 0;
}

/* Function gets an address of empty array with size of 1,
   and realloc according to user inputs, until frac is (0/0) */
int getFractionsInput(fraction *fractionArr[])
{
	int arrSize = 0;
	fraction frac; // temp fraction for testing user input before realloc.
	while (1) {
		if (arrSize > 0 && frac.denominator == 0) {
			printf("\nZeroDivisionError: can't divide by 0, please insert again...\n");
			arrSize--;
		}
		else if (arrSize > 0) { // inserting last inputted fraction.
			insertFractionToArr(fractionArr, &frac, arrSize - 1);
		}
		printf("%d (mone mehane): ", arrSize + 1);
		// assume user input ints (noted in requirements).
		scanf("%d %d", &frac.numerator, &frac.denominator);
		if (frac.numerator == 0 && frac.denominator == 0)
			break;
		arrSize++;
	}
	return arrSize;
}

/* Function gets address of fraction array and a new frac
   and realloc new slot in order to insert the new frac. */
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
		// assinning the pointer to the "new"(not always changes) location.
		*fractionArr = tempP;
	}
	(*fractionArr)[index] = *frac; // inserting the new frac to array.
}

/* Function gets an array of fractions and it's size and a pointer to empty fraction,
   and retruns the simplified sum of all fractions in "*simplifiedFrac"*/
void simplify(fraction fractionArr[], int size, fraction *simplifiedFrac)
{
	int i, greatDivder = 1, gcdOfFrac;
	for (i = 0; i < size; i++) // calculating the greatest common denominator.
		greatDivder *= fractionArr[i].denominator;
	for (i = 0; i < size; i++) {
		fractionArr[i].numerator *= greatDivder / fractionArr[i].denominator;
		fractionArr[i].denominator = greatDivder;
	}
	simplifiedFrac->denominator = greatDivder;
	simplifiedFrac->numerator = 0; // reset
	for (i = 0; i < size; i++) // adding all numerators to one.
		simplifiedFrac->numerator += fractionArr[i].numerator;

	gcdOfFrac = gcd(simplifiedFrac->numerator, simplifiedFrac->denominator);
	simplifiedFrac->numerator /= gcdOfFrac;
	simplifiedFrac->denominator /= gcdOfFrac;
}

/* Function returns the gcd between two numbers */
int gcd(int a, int b) {
	return (b == 0) ? a : gcd(b, a % b);
}

/* Function prints all fractions in oneline with '+' between each one */
void printFractions(fraction fractionArr[], int size)
{
	int i;
	for (i = 0; i < size; i++) {
		printf("%d/%d", fractionArr[i].numerator, fractionArr[i].denominator);
		if (i < size - 1) // not last fraction in arr
			printf(" + ");
	}
}

/* Function prints the result (simplified fraction) */
void printSimplied(fraction *simplifiedFrac)
{
	int intNum, moduleNum;
	// printing fraction as is.
	printf(" = %d/%d = ", simplifiedFrac->numerator, simplifiedFrac->denominator);
	intNum = simplifiedFrac->numerator / simplifiedFrac->denominator;
	moduleNum = simplifiedFrac->numerator % simplifiedFrac->denominator;
	// checking if possible to break it to int and (if needed) a fraction
	if (intNum != 0)
		printf("%d\n", intNum);
	if (moduleNum != 0)
		printf("%s%d/%d\n", (intNum == 0) ? "" : " and ", moduleNum, simplifiedFrac->denominator);
}
