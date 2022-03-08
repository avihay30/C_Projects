#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <math.h>

void f(char, int);

int main()
{
    char character;
    int number;

    printf("Enter a character and integer, please: ");
    scanf("%c %d", &character, &number);
    f(character, number);

	return 0;
}

void f(char c, int num) 
{
    int i;
    for (i = 0; i < num; i++)
    {
        int j;
        for (j = 0; j < num - i; j++)
            printf("%c", c);
        printf("\n");
    }
}