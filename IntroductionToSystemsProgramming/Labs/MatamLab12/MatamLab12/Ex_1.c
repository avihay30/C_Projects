#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>

// structures defining
typedef struct Item
{
	int num;
	struct Item* next;
} Item;

typedef struct List
{
	Item* head;
	Item* tail;
	int count;

}List;

// functions defining
void Error_Msg(char*);
void AddAsFirst(Item*, List*);
void AddAsLast(Item*, List*);
void MoveToAnotherList(List*, List*, List*);
void CreateList(List*, FILE*);
void PrintItem(Item*);
void PrintList(List*, char*);
void DeleteList(List*);

int main()
{
	List L, Posit, Negat;
	FILE* in = fopen("ThreeLists.txt", "rt");
	if (in == NULL)
		Error_Msg("input file is wrong");
	L.head = NULL;
	L.tail = NULL;
	L.count = 0;

	/* reset all the data in each structure */
	Posit.head = NULL;
	Posit.tail = NULL;
	Posit.count = 0;

	Negat.head = NULL;
	Negat.tail = NULL;
	Negat.count = 0;

	CreateList(&L, in);
	PrintList(&L, "\nMy List:\n");
	MoveToAnotherList(&L, &Posit, &Negat);
	PrintList(&Posit, "\n\nThe Positive List:\n");
	PrintList(&Negat, "\n\nThe Negative List:\n\n");

	fclose(in);
	DeleteList(&Posit);
	DeleteList(&Negat);
	return 0;
}


/* Function gets a string to print before exiting the program */
void Error_Msg(char* msg)
{
	printf("\n%s", msg);
	exit(1);
}

/* Function gets a pointer of a List and a FILE. 
   Puts every value from the FILE to the List in the FILE's order */
void CreateList(List* L, FILE* f)
{
	int value;
	Item* temp;
	/* as long we can read a value from the FILE */
	while (fscanf(f, "%d", &value) == 1)
	{
		/* allocate for the temp its size */
		temp = (Item*)malloc(sizeof(struct Item));
		if (temp == NULL)
		{
			DeleteList(L);
			Error_Msg("Memmory!");
		}

		temp->num = value;
		temp->next = NULL;
		/* if this is the first number in the List */
		if (L->head == NULL)
			L->head = temp;
		else
			L->tail->next = temp; // set the temp as the number after the tail
		/* set tail as temp */
		L->tail = temp;
		L->count++;
	}
}

/* Function gets a pointer of an Item and prints it's number */
void PrintItem(Item* node)
{
	printf("%d--> ", node->num);
}

/* Function gets a pointer of a list and a string. 
   Prints the string and the number of every Item in list. */
void PrintList(List* L, char* title)
{
	Item* temp = L->head; 
	printf("%s", title);
	while (temp) // while temp != NULL
	{
		PrintItem(temp); // send to the function the current head
		temp = temp->next; // set the temp as the next Item
	}
	printf("\nThere are %d items in the list", L->count);
}

/* Function gets a pointer of an Item and a list.
   insert the Item to beggining of list. */
void AddAsFirst(Item* ptr, List* L)
{
	ptr->next = L->head;
	L->head = ptr; 
	L->count++;
}

/* Fuction gets a pointer of an Item and of a list.
   inserts the Item to end of list. */
void AddAsLast(Item* ptr, List* L)
{
	ptr->next = NULL;
	if (L->head == NULL) // if first Item in list
		L->head = ptr;
	else
		L->tail->next = ptr;
	L->tail = ptr;
	L->count++;
}

/* Function gets a 3 pointers of List's.
   check's for every Item in 'orig' if its positive\negitive,
   and sends that Item to the proper list. */
void MoveToAnotherList(List* orig, List* posi, List* neg)
{
	Item* temp;
	while (orig->head != NULL) {
		temp = orig->head;
		orig->head = orig->head->next;

		if (temp->num >= 0) 
			AddAsLast(temp, posi);

		else // orig->head->num is negative
			AddAsFirst(temp, neg);
		
		orig->count--;
	}
	orig->tail = NULL; // reset tail of 'orig'
}

// Function gets a pointer of a List and deletes every Item in the list.
void DeleteList(List* L)
{
	Item* temp;
	while (L->head != NULL) {
		temp = L->head;
		L->head = L->head->next;
		free(temp);
	}
	/* reset count and tail of list */
	L->count = 0;
	L->tail = NULL;
}