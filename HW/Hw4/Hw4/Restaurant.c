#include "Restaurant.h"

/* Function addes products to the *end* of the list
   returns the tail of the list. */
void CreateProducts(pList prodList)
{
	FILE* f;
	float quantity, price;
	char dummyName[MAX_NAME_SIZE + 100]; // for testing.
	int numOfInputs;
	pProduct tempProd;
	f = fopen("Manot.txt", "rt");
	checkAllocation(f, FILE_ERR, prodList);
	while ((numOfInputs = fscanf(f, "%s %f %f", dummyName, &quantity, &price)) != EOF) {
		if (numOfInputs != 3 || !isProductValid(dummyName, quantity, price))
			// passing to function NULL on perpose to free all memory if need and exit.
			checkAllocation(NULL, FILE_READ_ERR, prodList);
		
		checkName(prodList, dummyName);
		// building product [all inputs from file are valid casting is safe]
		tempProd = (pProduct)malloc(sizeof(product));
		checkAllocation(tempProd, ALLOC_ERR, prodList);
		tempProd->ProductName = (char*)malloc(strlen(dummyName) + 1);
		checkAllocation(tempProd->ProductName, ALLOC_ERR, prodList);
		strcpy(tempProd->ProductName, dummyName);
		tempProd->Quantity = (int)quantity;
		tempProd->Price = (int)price;
		tempProd->next = NULL;
		
		// inserting to end of list
		if (prodList->head == NULL)
			prodList->head = tempProd;
		else
			prodList->tail->next = tempProd;
		prodList->tail = tempProd;
		prodList->size++;
	}
}

void AddItems(char* ProductName, int Quantity)
{

}

void OrderItem(int TableNumber, char* ProductName, int Quantity)
{

}

void RemoveItem(int TableNumber)
{

}

void RemoveTable(int TableNumber)
{

}