#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <math.h>

int main()
{
	int num, is_negative = 0, counter = 0;
	
    printf("Enter an integer number, please: ");
	scanf("%d", &num);

    if (num == 0)
    {
        printf("Infinity");
        return 0;
    }
    if (num < 0)
    {
        /* changing the variable "is_negative" to be "true". */ 
        is_negative = 1;
        num = abs(num);
    }

    int i;
    for (i = 1; i <= num / 2; i++)
    {
        if (num % i == 0)
        {
            printf("%d ", i);
            counter++;
        }
    }
    /* is_negative == ture */
    if (is_negative == 1) {
        printf("%d ", num);
        counter++;
    }

    printf("\ncount = %d", counter);
	return 0;
}