#include "Stack_Header_Array.h"

/* Function gets a stack and its "fixed" size 
   and initialize the new stack with capacity of size elements */
void InitStack(PStack s, int size)
{
	int delNum;
	s->top = -1;
	s->Array = (int*)malloc(size * sizeof(int));
	if (s->Array == NULL) {
		printf(ALLOC_ERR);
		while(Pop(s, &delNum)) {}
	}
	s->size = size;
	s->count = 0;
}
/* Function gets a stack and a number to push 
   and adds a new member(number) to array of the stack */
void Push(PStack s, int new_elem)
{
	if (s->size >= (s->count + 1)) {
		s->Array[s->count++] = new_elem;
		s->top++;
	}
	else
		printf("\nThe stack is full.");
}

/* Function get a stack and dummy int pointer and deletes a member from the array 
   of the stack and "return" the deleted value using dummy int */
int Pop(PStack s, int *del_value)
{
	if (s->count == 0) {
		printf("\nThe Stack is empty\n");
		return 0;
	}
	*del_value = s->Array[s->top--];
	s->count--;
	return 1;
}