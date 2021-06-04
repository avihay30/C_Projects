#include "Restaurant.h"

// Function checks if pointer allocation is invalid and free memory needed and exit. 
void checkAllocation(void* pToCheck, char* message, pList productsList)
{
	if (pToCheck == NULL) {
		fprintf(stderr, "%s\n", message);
		freeAll(productsList);
		exit(1); // closes file automaticaly
	}
}

// Function gets an pointer and free all allocated memory.
void freeAll(pList prodList)
{
	pProduct tempP;
	while (prodList->head != NULL) {
		free(prodList->head->ProductName);
		tempP = prodList->head->next;
		free(prodList->head);
		prodList->head = tempP;
	}
}

Bool isProductValid(char* name, float quantity, float price)
{
	if (strlen(name) == 0 || strlen(name) > MAX_NAME_SIZE) // checking name
		return FALSE;
	if (quantity < 0.0 || floor(quantity) != ceil(quantity)) // if negative or not int.
		return FALSE;
	if (price < 0.0 || floor(price) != ceil(price)) // if negative or not int.
		return FALSE;

	return TRUE; // logical else
}

Bool isOperationValid(float op)
{
	// checks int op and its required boundries.
	if (op < 0.0 || op > 5.0 || floor(op) != ceil(op))
		return FALSE;
	return TRUE; // logical else
}

/* Function checks if a name exits in list,
   if true free all memory and output error message and exits */
void checkName(pList prodList, char* name)
{
	pProduct head = prodList->head;
	while (head != NULL) {
		if (strcmp(head->ProductName, name) == 0)
			checkAllocation(NULL, NAME_DUPL_ERR, prodList);
		head = head->next;
	}
}