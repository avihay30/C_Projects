#ifndef _Stack_Header_Array
#define _Stack_Header_Array

#include <stdio.h>
#include <stdlib.h>

#define ALLOC_ERR "AlloctionError: The program couldn't allocate memory!\n"

typedef struct Stack {
int top; // index of the last member in array of a stack;
int* Array; // pointer to array of members of a stack
int size; // size of the array of a stack
int count; // the current number of members
}Stack,*PStack;

void InitStack(PStack s, int size);
void Push(PStack s, int new_elem);
int Pop(PStack s, int *del_value);

#endif // _Stack_Header_Array