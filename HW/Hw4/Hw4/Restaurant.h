#ifndef  _Restaurant
#define _Restaurant

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define ALLOC_ERR "AlloctionError: The program couldn't allocate memory!\n"
#define FILE_ERR "FileError: The program couldn't create/read file!\n"
#define FILE_READ_ERR "Read-File Error: the input file is empty or invalid!!\n"
#define INSTRUCTIONS_ERR "Read-File Error: the instructions file is empty or invalid!!\n"
#define NAME_DUPL_ERR "Read-File Error: the Mantot file has duplicte %s product!!\n"
#define INVALID_TABLE_NUM "Invalid table number!\n"
#define NUMBER_OF_TABLES 50
#define MAX_NAME_SIZE 50
#define KITCHEN_CREATED "The kitchen was created\n"
#define ADD_TO_KITCHEN "%d %s were added to the kitchen\n" // %d- NO. prod, %s- prod name
#define PRODUCT_UNAVILEBLE "We don`t have %s, sorry!\n" // %s- prod name
#define ADD_TO_TABLE "%d %s were added to the table number %d\n" // %d- NO. prod, %s- prod name, %d- NO. table
#define PRODUCT_RETURNED "%d %s was returned to the kitchen from table number %d\n" // %d- NO. prod, %s- prod name, %d- NO. table
#define TABLE_DIDNT_ORDERD "The table number %d is not ordered yet\n" // %d NO. table
#define PRICE "%d nis + %d nis for tips, please!\n" // %d- total price, %d- tip

typedef enum { FALSE, TRUE } Bool;

// define product(node) struct
typedef struct product
{
	char* ProductName; // no duplicates
	int Price; // positive definit
	int Quantity; // positive definit
	struct product* next;
}product, *pProduct;

typedef struct list
{
	pProduct head;
	pProduct tail;
}list, *pList;

// define order(node) struct
typedef struct order
{
	pProduct product; // no duplicates
	int Quantity; // positive definit
	struct order* next;
}order, *pOrder;

typedef struct orders
{
	pOrder head;
	Bool isCanceledOrder;
}orders, *pOrders;

typedef struct restaurant
{
	list kitchen;
	orders tables[NUMBER_OF_TABLES];
}restaurant, *pRestaurant;

void CreateProducts(pList);
void AddItems(pList, char*, float);
void OrderItem(pRestaurant, float, char*, float);
void RemoveItem(orders[], float);
void RemoveTable(orders[], float);

Bool checkTableNumber(float);
void checkAllocation(void*, char*,void* ,void(*)(void*));
void freeKitchen(pList);
void freeAll(pRestaurant);
Bool isProductNameValid(char* name);
Bool isIntPositive(float);
Bool isOperationValid(float);
pProduct getProductPtr(pList , char*);

#endif //  _Restaurant