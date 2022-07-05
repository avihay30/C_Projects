#include "Stack_Header.h"

/* The function gets a pointer of a stack and an int for a new number.
   add a new member to list of the stack */
void Push(PStack s, int new_elem)
{
	Item* temp;
	int delNum;
	temp = (Item*)malloc(sizeof(Item));
	if (temp == NULL) {
		printf(ALLOC_ERR);
		while (Pop(s, &delNum)) {}
	}
	temp->num = new_elem;
	temp->next = s->head;
	s->head = temp;
	// chenging the head
	s->size++;
}

/* The function gets a pointer of a stack and a dummy int pointer for a number.
   delete member from the stack and "return" the deleted value using int *del_value*/
int Pop(PStack s, int* del_value)
{
	Item* temp;
	if (s->head == NULL) {
		return 0;
	}

	*del_value = s->head->num;
	// moving and deleting head.
	temp = s->head->next;
	free(s->head);
	s->head = temp;
	s->size--;
	return 1;
}