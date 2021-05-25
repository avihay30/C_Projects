#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void Error_Msg(char* str);
void printStudentsToFile(FILE*, char*, double);

int main()
{
	char name[7], dep[5], grade1[4], grade2[4];
	double avg = 0;
	FILE *in,*out;
	
	if((in=fopen("Students.txt","r")) == NULL)
             Error_Msg("The input file is wrong");
	if((out=fopen("StudentsNew.txt","w")) == NULL)
             Error_Msg("The output file is wrong");
	while (fgets(name, 7, in) != NULL) {
		fgets(dep, 5, in);
		if (strcmp(dep, "Comp") == 0) {
			fgets(grade1, 4, in);
			fgets(grade2, 4, in);
			avg = (atoi(grade1) + atoi(grade2)) / 2.0;
			printStudentsToFile(out, name, avg);
		}
		else // skip unwonted text.
			fseek(in, 6, SEEK_CUR); // 8 represents the amount of bytes to skip.
	}

	fclose(in);
    fclose(out);
    return 0;
}


void Error_Msg(char* str)
{
	printf("\n%s",str);
        exit(1);
}

void printStudentsToFile(FILE* out, char* name, double avg)
{
	fprintf(out, "%s %.2lf\n", name, avg);
}