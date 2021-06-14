#ifndef _Stack_Header
#define _Stack_Header

#include <stdio.h>
#include <stdlib.h>

#define NUM 5
#define ALLOC_ERR "AlloctionError: The program couldn't allocate memory!\n"

typedef struct Item {
	int num;
	struct Item* next;
}Item;

typedef struct Stack {
	Item* head;
	int size; //a current number of items
}Stack, * PStack;

void Push(PStack s, int   new_elem); //add a new member to list of the stack
int Pop(PStack s, int* del_value); //delete member from the stack and  “return” the deleted value  using  int * del_value

#endif //_Stack_Header