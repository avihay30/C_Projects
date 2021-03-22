#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <math.h>

int main()
{
	char c;
	while (1)
	{
		int num1, num2;
		printf("Enter your char: ");
		scanf("%c", &c);
		rewind(stdin);

		if (c == 'Q' || c == 'q')
		{
			printf("Finish");
			break;
		}
		if (c == 'A' || c == 'a' || c == 'm' || c == 'M' || c == '^' || c == '*')
		{
			printf("Enter your numbers: ");
			scanf("%d %d", &num1, &num2);
			rewind(stdin);
		}
		printf("\n");
		switch (c)
		{
		case 'A': case 'a':
			printf("The average of %d, %d is: %g\n", num1, num2, (num1 + num2) / 2.0);
			break;
		case '*':
			printf("The multiplication of %d, %d is: %d\n", num1, num2, num1 * num2);
			break;
		case 'm':
			printf("The minimum of %d, %d is: %d\n", num1, num2, (num1 < num2 ? num1 : num2));
			break;
		case 'M':
			printf("The maximum of %d, %d is: %d\n", num1, num2, (num1 > num2 ? num1 : num2));
			break;
		case '^':
			if (num1 == 0 && num2 == 0)
			{
				printf("Sorry, you can't do power of 0 on base 0\n");
				break;
			}
			printf("The power of %d in exponent %d is: %g\n", num1, num2, pow(num1, num2));
			break;
		default:
			printf("Error\n");
			break;
		}
	}
	return 0;
}