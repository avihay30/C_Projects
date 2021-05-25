#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>

//int getBit(unsigned int, int);
int Bit_Count(unsigned int x);
void Bin_Print(unsigned int x);

int main()
 {
	unsigned int x;

	printf("Enter a number");
    scanf("%d",&x);
	printf("There are %d bits equal to one in %d\n", Bit_Count(x),x);
	printf("The binary representation of %d is ",x); 
	Bin_Print(x);

    return 0;
}

/* int getBit(unsigned int number, int bit)
{
	unsigned int unity = 1, res;
	res = number & (unity << bit);
	return (res != 0);
} */

int Bit_Count(unsigned int x)
 {
	int n = 8 * sizeof(unsigned int);
	int i, cnt = 0;
	unsigned int unity, res;
	for (i = 0; i < n; i++) {
		res = x & (unity << i);
		cnt += (res != 0);
	}
	return cnt;
 }

void Bin_Print(unsigned int x)   
 {
	int n = 8 * sizeof(unsigned int);
	int i;
	unsigned int unity = 1, res;
	for (i = n - 1; i >= 0; i--) {
		res = x & (unity << i);
		printf("%d", (res != 0));
	}
 }
