#define _CRT_SECURE_NO_WARNINGS

#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#define MAX 256

typedef struct Book
{
	char code[10]; // 9 + '/0'
	char* name;
} Book;

typedef struct Library
{
	char name[MAX]; // 255 + '/0'
	int numOfBooks;
	Book* books;
} Library;

void Error_Msg(char*);
void input_book(Book* B,FILE *in);
void input_library(Library *L,FILE *in);
void output_book(Book* B,FILE *out);
void output_library(Library* L,FILE *out);

int main()
{
	FILE *in,*out;
	Library Libr;        
	int i;
	
	in = fopen("one.txt", "rt");
	if (in == NULL)
		Error_Msg("The input file is wrong");
	input_library(&Libr,in);
	fclose(in);

	out = fopen("output.txt", "wt");
	if (in == NULL)
		Error_Msg("The output file is wrong");

	output_library(&Libr,out);
	fclose(out);
        
    for(i=0;i<Libr.numOfBooks; i++)   /* free the memory!!!*/
		free(Libr.books[i].name);
	free(Libr.books);
 
    return 0;
}

void Error_Msg(char* str)
{
	printf("\n%s", str);
	exit(1);
}

/* Function gets a pointer to a 'Book' and pointer of a FILE
   assign to the book a code and a name */
void input_book(Book* B, FILE* in)
{
	char tempName[MAX];
	fscanf(in, "%s", B->code);
	fscanf(in, "%s", tempName);
	B->name = (char*)malloc(strlen(tempName) + 1); // sizeof(char) = 1
	if (B->name == NULL) /* if the Name is NULL */
		return; /* return to 'input_library' */
	strcpy(B->name, tempName); // logical else.
}

/* Function gets a pointer of a Library and the pointer of a FILE and
   Fills the Library with the books using 'input_book' */
void input_library(Library* L, FILE* in)
{
	int i, j;
	fscanf(in, "%s", L->name);
	fscanf(in, "%d", &(L->numOfBooks));
	L->books = (Book*)malloc(L->numOfBooks * sizeof(Book));
	if (L->books == NULL)
		Error_Msg("Allocation Error!");
	for (i = 0; i < L->numOfBooks; i++) {
		input_book(&(L->books[i]), in);
		// incase the current books name is NULL
		if (L->books[i].name == NULL)
		{
			for (j = 0; j < i; j++)
			{
				free(L->books[j].name);
			}
			free(L->books);
			Error_Msg("Allocation Error!");
		}
	}
}

void output_book(Book* B, FILE* out)
{
	fprintf(out, "%-8s %s\n", B->code, B->name);
}

void output_library(Library* L, FILE* out)
{
	int i;
	fprintf(out, "%s\n", L->name);
	for (i = 0; i < L->numOfBooks; i++) {
		output_book(&(L->books[i]), out);
	}
}

