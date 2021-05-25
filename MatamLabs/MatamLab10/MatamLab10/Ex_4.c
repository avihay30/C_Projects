#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>

unsigned char check_ms(unsigned char);
unsigned char change_bit(unsigned char, int);
void BinPrint(unsigned char ch);

int main()
{
	unsigned char num1 = 102, num2 = 58;
	int n;

	printf("\nThe first part :");
	printf("\nThe number is %d   ", num1);
	BinPrint(num1);
	num1 = check_ms(num1);
	printf("\nThe new number is: %d   ", num1);
	BinPrint(num1);

	printf("\n\nThe second part :");
	printf("\nThe number is %d   ", num2);
	BinPrint(num2);
	printf("\nEnter a number of the bit to change(1-8)");
	scanf("%d",&n);
	num2 = change_bit(num2, n);
	printf("\nThe new number is: %d   ", num2);
	BinPrint(num2);
	return 0;
}

unsigned char check_ms(unsigned char ch)
{
	int n = 8 * sizeof(unsigned char);
	unsigned char unity = 1;
	return ch | (unity << (n-1));
}

unsigned char change_bit(unsigned char ch, int bit)
{
	int n = 8 * sizeof(unsigned char);
	unsigned char unity = 1, res;
	unity = unity << (n - bit);
	res = ch & unity;
	if (res != 0)
		return ch & ~(unity);
	return ch | unity; // logical else
}

void BinPrint(unsigned char ch)
{
	int n = 8 * sizeof(unsigned char);
	int i;
	unsigned char unity = 1, res;
	for (i = n - 1; i >= 0; i--) {
		res = ch & (unity << i);
		printf("%d", (res != 0));
	}
}