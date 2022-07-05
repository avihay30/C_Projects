#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <math.h>

int main()
{
	int weight, height;
	/* Getting the information of the user's weight and height */
	printf("Please enter weight (in kg) and height (in cm), respectively: ");
	scanf("%d %d", &weight, &height);

	/* calculate his BMI(BMI = weight(kg) / (height(m)) ^ 2) */
	double bmi = weight / pow((height / 100.0), 2);

	/* printing the weight status according to the bmi calculation */
	if (bmi < 18.5)
		printf("Underweight");
	else if (bmi < 25)
		printf("Normal weight.");
	else if (bmi < 30)
		printf("Increased weight.");
	else if (bmi < 40)
		printf("Obese.");
	else
		printf("Very high obese.");
	
	return 0;
}