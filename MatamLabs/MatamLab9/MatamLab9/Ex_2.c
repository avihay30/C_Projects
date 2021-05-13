#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct
{
    char name[20];
    int grades[3];
}student;

void Error_Msg(char*);
int InputData(student**, FILE*);
void OutputData(int, student*, FILE*);
int getMaxGrade(int[]);

int main()
{
    FILE* fp;
    student* arr;
    int size;
    if ((fp = fopen("Students.txt", "rt")) == NULL)
        Error_Msg("The input file is wrong");
    size = InputData(&arr, fp);
    fclose(fp);

    if ((fp = fopen("Students.txt", "wt")) == NULL)
    {
        free(arr);
        Error_Msg("The output file is wrong");
    }
    OutputData(size, arr, fp);
    fclose(fp);

    free(arr);
    return 0;
}

int InputData(student** p_array, FILE* fp)
{
    student* arr, *temp;
    int i = 1;
    char tempName[20];

    arr = (student*)malloc(sizeof(student));
    if (arr == NULL)
        Error_Msg("AllocationError: couldn't allocate memory");

     // we chenge the previus while because it 
    while (fscanf(fp, "%s ", tempName) != EOF) {
        if (i > 1) {
            temp = (student*)realloc(arr, i * sizeof(student));
            if (temp == NULL) {
                free(arr);
                Error_Msg("AllocationError: couldn't allocate memory");
            }
            arr = temp;
        }
        strcpy(arr[i - 1].name, tempName);
        fscanf(fp, "%d %d %d", &arr[i - 1].grades[0], &arr[i - 1].grades[1], &arr[i - 1].grades[2]);
        i++;
    }
    *p_array = arr;
 
    return i - 1;
}

void OutputData(int arr_size, student* arr, FILE* fp)
{
    int i;
    for (i = 0; i < arr_size; i++) {
        fprintf(fp, "%s %d\n", arr[i].name, getMaxGrade(arr[i].grades));
    }
}

int getMaxGrade(int grades[])
{
    int max = (grades[0] > grades[1]) ? grades[0] : grades[1];
    return (max > grades[2]) ? max : grades[2];
}

void Error_Msg(char* msg)
{
    printf("\n%s", msg);
    exit(1);
}