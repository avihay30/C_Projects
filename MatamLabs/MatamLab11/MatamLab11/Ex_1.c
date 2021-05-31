#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 5

typedef struct Item {
	int code;
	char name[10];
	struct Item *next;
} Item;


int main()
{
	int i;
	Item *Head=NULL, *temp;
	for(i=1;i<=N;i++){		
		printf("Enter a new code and name: ");
		temp = (Item*)malloc(sizeof(Item));
		if (temp == NULL) {
			printf("Allocation Error");
			while (Head != NULL) { // free whole list.
				temp = Head->next;
				free(Head);
				Head = temp;
			}
			exit(1);
		}
		scanf("%d %s", &(temp->code), temp->name);
		printf("\n");
		// inserting to list.
		temp->next = Head;
		Head = temp;
	}
	
	printf("\n\nThe List is:  ");
	while(temp != NULL) {
		printf("%d,%s --> ", temp->code, temp->name);
		temp = temp->next; // moving temp over the all list.
	}

	while(Head != NULL){
		temp = Head->next;
		free(Head);
		Head = temp;
	}
	return 0;
}