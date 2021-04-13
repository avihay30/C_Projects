#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

void Set_Mid(double x1, double y1, double x2, double y2, double* p_mx, double* p_my);

int main() 
{
	double x1, y1, x2, y2;
	double m_x, m_y;
	printf("Please enter two points is space (x,y):\nEnter first point: ");
	scanf("%lf %lf", &x1, &y1);
	/* cleanning keyboard buffer */
	rewind(stdin);
	printf("Enter second point: ");
	scanf("%lf %lf", &x2, &y2);

	/* Calling Set_Mid function with the addresses of m_x and m_y */
	Set_Mid(x1, y1, x2, y2, &m_x, &m_y);
	/* printing the middle point without leftovers zeros after decimel point*/
	printf("The middle point is: (%g,%g)", m_x, m_y);

	return 0;
}

void Set_Mid(double x1, double y1, double x2, double y2, double* p_mx, double* p_my)
{
	/* changing the value inside the p_mx/y pointers */
	*p_mx = (x1 + x2) / 2;
	*p_my = (y1 + y2) / 2;
}