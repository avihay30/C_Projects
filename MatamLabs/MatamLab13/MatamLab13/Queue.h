#ifndef _Queue
#define _Queue

#include <stdio.h>
#include <stdlib.h>

#define NUM 5
#define ALLOC_ERR "AlloctionError: The program couldn't allocate memory!\n"

typedef struct Item{
	int num; 
	struct Item* next;
}Item;

typedef struct Que {
	Item* head, *tail;
	int size; //a current number of items
}Queue, *PQue;

void Enqueue(PQue Q, int new_elem); //add a new member to list of the queue 
int Dequeue(PQue Q, int* del_value); //delete member from the queue and  "return" the deleted value  using int 

#endif // _Queue