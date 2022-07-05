#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node
{
	char code[11];
	char *name;
	char Dep[4];
	int marks[3];
	float avg;
	struct node* next;
}std;

void Error_Msg(char* str);
std* FromFileToList(std* head,FILE *in);
std* Delete_Stud(std* toDel,std* head);
std* DeleteList(std* head);
void PrintList(std* head);
std* FindMax(std* head);

int main()
{
	int i;
	FILE *f;
	std *Head=NULL, *temp;
	if((f=fopen("List.txt","rt")) == NULL)
		Error_Msg("input error");
	Head=FromFileToList(Head,f);
	if (Head == NULL)
		Error_Msg("The input file is empty");
	
	fclose(f);
	printf("\nThe list is:");
	PrintList(Head);
	
	temp = FindMax(Head);
    printf("\n\nthe student with max average:\n");
	printf("%s %s %s ",temp->code,temp->name,temp->Dep);
    for(i=0;i<3;i++)
		printf(" %d ",temp->marks[i]);

	printf("\n\nThe list after change:");
	Head=Delete_Stud(FindMax(Head),Head);
	PrintList(Head);
	Head = DeleteList(Head); /*Head = NULL */
    return 0;
}

void Error_Msg(char* str)
{
	printf("\n%s",str);
    exit(1);
}

/* Function gets a file and head of empty list
   and fills the list with the values of the list. */
std* FromFileToList(std* head, FILE* in)
{
	char tempName[256];
	std temp, *ptemp;
	while (fscanf(in, "%s %s %s %d %d %d", temp.code, tempName, temp.Dep, &temp.marks[0], &temp.marks[1], &temp.marks[2]) != EOF) {
		ptemp = (std*)malloc(sizeof(std));
		if (ptemp == NULL) {
			DeleteList(head);
			Error_Msg("\nAllcation error");
		}
		*ptemp = temp;
		ptemp->name = (char*)malloc(strlen(tempName) + 1); // + 1 for '\0'
		if (ptemp->name == NULL) {
			DeleteList(head);
			Error_Msg("\nAllcation error");
		}
		strcpy(ptemp->name, tempName);
		// 3 is num of grades.
		ptemp->avg = (ptemp->marks[0] + ptemp->marks[1] + ptemp->marks[2]) / 3.0;
		ptemp->next = head;
		head = ptemp;
	}
	return head;
}

/* Function gets head of list and prints the list. */
void PrintList(std* head)
{
	int i;
	while (head != NULL) {
		printf("\n%s %s %s", head->code, head->name, head->Dep);
		for (i = 0; i < 3; i++) // 3 is number of grades.
			printf(" %d", head->marks[i]);
		head = head->next; // moving head over the all list.
	}
}

/* Function gets head of list and returns element with max avg in list. */
std* FindMax(std* head)
{
	std* maxStud = head;
	while (head != NULL) {
		maxStud = (maxStud->avg < head->avg) ? head : maxStud;
		head = head->next;
	}
	return maxStud;
}

/* Function gets head of list and to delete node 
   and returns the head of the new list. */
std* Delete_Stud(std* toDel,std* head)
{
	std* temp = head;
	if(head == NULL)
		return NULL;
  
	if(toDel==head)
	{
		head = head->next;
	}
	else
	{
		while (temp->next != toDel)
			temp = temp->next; // moving head forward.
		temp->next = toDel->next;
	}
	free(toDel->name);
	free(toDel);
	return head;
}

/* Function gets head of list and free all list 
   returns the head of the empty list(NULL). */
std* DeleteList(std* head)
{
	std* temp;
	while (head != NULL) {
		temp = head->next;
		free(head->name);
		free(head);
		head = temp;
	}
	return head;
}
