#include "Queue.h"

int main()
{
	Queue queue;
	Item* temp;
	int i, delNum, arr[NUM] = { 1, 2, 3, 4, 5 }; 
	queue.head = queue.tail = NULL;
	queue.size = 0;

	for (i = 0; i < NUM; i++) {
		Enqueue(&queue, arr[i]);
	}
	if (!Dequeue(&queue, &delNum)) {
		printf("cannot push, Queue is empty");
		return 1;
	}
	printf("The deleted item is %d\n", delNum);
	temp = queue.head;
	printf("The numbers in the queue are:\n\t");
	for (i = 0; i < queue.size; i++) { // printing stack
		printf("%d --> ", temp->num);
		temp = temp->next;
	}
	while (Dequeue(&queue, &delNum)) {} // free all stuck.
	return 0;
}