#include "Stack_Header.h"

int main()
{
	Stack stack;
	Item* temp;
	int i, delNum, arr[NUM] = { 1, 2, 3, 4, 5 }; 
	stack.head = NULL;
	stack.size = 0;
	for (i = 0; i < NUM; i++) {
		Push(&stack, arr[i]);
	}
	if (!Pop(&stack, &delNum)) {
		printf("cannot push, Stuck is empty");
		return 1;
	}
	printf("The deleted item is %d\n", delNum);
	temp = stack.head;
	printf("The numbers in the stack are:\n");
	for (i = 0; i < stack.size; i++) { // printing stack
		printf("\t%d\n", temp->num);
		printf("\t^\n\t|\n");
		temp = temp->next;
	}
	while (Pop(&stack, &delNum)) {} // free all stuck.
	return 0;
}