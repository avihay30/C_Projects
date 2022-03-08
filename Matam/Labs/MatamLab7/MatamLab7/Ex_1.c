#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <math.h>

typedef struct point {
	float x;
	float y;
} point;

typedef struct circle {
	point center;
	float radius;
} circle;

int isPointInCircle(circle*, point*);

#define TRUE 1
#define FALSE 0

int main() {
	point userPoint;
	circle userCircle;

	printf("Enter the coordinates of your point(x, y): ");
	scanf("%f %f", &userPoint.x, &userPoint.y);
	printf("\nEnter the radius and the center(x, y) of your circle: ");
	scanf("%f %f %f", &userCircle.radius, &userCircle.center.x, &userCircle.center.y);

	printf("\nThe point is %sincluded in the circle\n", 
		isPointInCircle(&userCircle, &userPoint) ? "" : "not ");

	return 0;
}

/* The function gets a pointer of a specific circle and point strucs
   and returns whether the point is inside the circle or not. */
int isPointInCircle(circle* circle, point* point)
{
	// formula for finding distance from center of circle sqrt((x-x0)^2 + (y-y0)^2))
	double disFromCenter = sqrt(
		pow(circle -> center.x - point -> x, 2) +
		pow(circle -> center.y - point -> y, 2));

	return (disFromCenter < circle -> radius) ? TRUE : FALSE;
}