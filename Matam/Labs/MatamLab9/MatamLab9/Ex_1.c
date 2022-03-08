#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>

typedef struct Person
{
	char ID[10];
	char F_name[11];
	char L_name[16];
	int Age;
	char Addr[51];
}Person;

void Error_Msg(char*);

int main()
{
	Person temp;
	FILE* in, * out;
	if ((in = fopen("the_first.txt", "rt")) == NULL)
		Error_Msg("IOError: the_first.txt file couldn't open!");
	if (fscanf(in, "%s %s %s %d %s", temp.ID, temp.F_name, temp.L_name, &temp.Age, temp.Addr) != 5)
		Error_Msg("data in the_first.txt is smaller than 'Preson' struct");
	fclose(in);

	if ((out = fopen("the_second.txt", "wt")) == NULL)
		Error_Msg("IOError: the_second.txt file couldn't open or created!");
	fprintf(out, "ID:%s\nFull name:%s %s\nAge:%d\nAddress:%s", temp.ID, temp.F_name, temp.L_name, temp.Age, temp.Addr);
	fclose(out);

	return 0;
}

void Error_Msg(char* msg)
{
	printf("\n%s", msg);
	exit(1);             /*Exit() closes any open files in the program*/
}