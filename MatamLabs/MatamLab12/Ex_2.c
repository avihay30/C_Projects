#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>

#define N 5

// structures defining
typedef struct Item
{
	int num;
	struct Item* next;
}*PItem;

// functions defining
void Error_Msg(char*);
void CreateListFromArray(PItem*, PItem*, int*);
void DeleteList(PItem*);
void ListDisplay(PItem);
int ListLength(PItem);

int main()
{
	int Arr[N] = { 3,4,1,0,8 };

	PItem list = NULL, tail = NULL;

	CreateListFromArray(&list, &tail, Arr);
	printf("The length of the list is %d members\n", ListLength(list));
	printf("\nThe list is:\n");
	ListDisplay(list);

	DeleteList(&list);
	tail = NULL;
	return 0;
}

// Function gets a string to print before exiting the program
void Error_Msg(char* msg)
{
	printf("\n%s", msg);
	exit(1);
}

/* Function gets two pointers of Iteams and one array.
   Creates a list from the int's in the array. */
void CreateListFromArray(PItem* head, PItem* tail, int* Arr)
{
	int i;
	PItem temp;
	for (i = 0; i < N; i++)
	{
		temp = (PItem)malloc(sizeof(struct Item));
		if (temp == NULL)
		{
			DeleteList(head);
			Error_Msg("Memmory!");
		}
		temp->num = Arr[i];
		temp->next = NULL;
		if (*head == NULL)
			*head = temp; // set current temp as head
		else
			(*tail)->next = temp;
		*tail = temp; // set the temp as the new tail
	}
}

/* Function gets a pointer of a Item 
   and deletes every Item that it points to. */
void DeleteList(PItem* head)
{
	PItem tmp = *head;
	while (*head)
	{
		tmp = *head;
		*head = (*head)->next;
		free(tmp);
	}
}
/* Recursive function that gets a pointer of an Item, 
   prints every Item until the end of the list. */
void ListDisplay(PItem q)
{
	if (q == NULL) { // exit case: end of list
		printf("//");
		return;
	} // logical else
	printf("%d--> ", q->num);
	ListDisplay(q->next); 
}

/* Recursive function that gets a pointer of an Item, 
   returns the amount of Item's in the list. */
int ListLength(PItem q)
{
	if (q == NULL) // exit case: end of list 
		return 0;
	return 1 + ListLength(q->next); // logical else
}