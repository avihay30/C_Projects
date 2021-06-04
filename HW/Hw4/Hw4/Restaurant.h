#ifndef  _Restaurant
#define _Restaurant

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define ALLOC_ERR "AlloctionError: The program couldn't allocate memory!"
#define FILE_ERR "FileError: The program couldn't create/read file!"
#define FILE_READ_ERR "Read-File Error: the input file is empty or invalid!!"
#define INSTRUCTIONS_ERR "Read-File Error: the instructions file is empty or invalid!!"
#define NAME_DUPL_ERR "Read-File Error: the Mantot file has duplicte product name!!"
#define NUMBER_OF_TABLES 50
#define MAX_NAME_SIZE 50
#define KITCHEN_CREATED "The kitchen was created"
#define ADD_TO_KITCHEN "%d %s were added to the kitchen" // %d- NO. prod, %s- prod name
#define PRODUCT_UNAVILEBLE "We don’t have %s, sorry!" // %s- prod name
#define ADD_TO_TABLE "%d %s were added to the table number %d" // %d- NO. prod, %s- prod name, %d- NO. table
#define PRODUCT_RETURNED "%d %s was returned to the kitchen from table number %d" // %d- NO. prod, %s- prod name, %d- NO. table
#define TABLE_DIDNT_ORDERD "The table number %d is not ordered yet" // %d NO. table
#define PRICE "%d %s. %d nis+%d nis for tips, please!" // %d- NO. prod, %s- prod name, %d- total price, %d- tip

typedef enum { FALSE, TRUE } Bool;

// define product(node) struct
typedef struct product
{
	char* ProductName; // no duplicates
	int Price; // positive definit
	int Quantity; // positive definit
	struct product* next;
} product, *pProduct;

typedef struct list
{
	pProduct head;
	pProduct tail;
	int size;
}list, *pList;


void CreateProducts(pList);
void AddItems(char*, int);
void OrderItem(int, char*, int);
void RemoveItem(int);
void RemoveTable(int);

void checkAllocation(void*, char*, pList);
void freeAll(pList);
Bool isProductValid(char*, float, float);
Bool isOperationValid(float);
void checkName(pList, char*);

#endif //  _Restaurant

