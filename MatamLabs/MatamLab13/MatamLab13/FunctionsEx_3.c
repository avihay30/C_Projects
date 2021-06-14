#include "Queue.h"

/* Function gets a queue and a number 
   and inserts new node(with number) to queue(to tail). */
void Enqueue(PQue q, int new_elem)
{
	Item *temp;
	int delNum;
	temp = (Item*)malloc(sizeof(Item));
	if (temp == NULL) {
		printf(ALLOC_ERR);
		while(Dequeue(q, &delNum)) {} // free all queue.
	}
	// inserting temp(node) to tail. 
	temp->num = new_elem;
	temp->next = NULL;
	if (q->head == NULL)
		q->head = temp;
	else
		q->tail->next = temp;
	q->tail = temp;
	q->size++;
}

/* Function gets a queue and a dummy int pointer and deletes the head element in queue 
   and assigning the removed element to the dummy int*/
int Dequeue(PQue q, int* del_value)
{
	Item* temp;
	if (q->head == NULL) { // if queue is empty.
		return 0;
	}
	
	*del_value = q->head->num;
	// moving and deleting head.
	temp = q->head->next;
	free(q->head);
	q->head = temp;
	q->size--;
	return 1;
}