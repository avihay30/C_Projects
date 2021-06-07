/* #include "Restaurant.h"

// Function checks if pointer allocation is invalid and free memory needed and exit. 
void checkAllocation(void* pToCheck, char* message, void* listToFree, void(*fp)(void*))
{
	if (pToCheck == NULL) {
		fprintf(stderr, "%s\n", message);
		fp(listToFree);
		exit(1); // closes file automaticaly
	}
}

// Function gets an pointer and free all allocated memory of kitchen.
void freeKitchen(pList prodList)
{
	pProduct tempP;
	while (prodList->head != NULL) {
		free(prodList->head->ProductName);
		tempP = prodList->head->next;
		free(prodList->head);
		prodList->head = tempP;
	}
}

void freeAll(pRestaurant rest)
{
	pOrders tables = rest->tables;
	int i;
	pOrder tempP;
	for (i = 0; i < NUMBER_OF_TABLES; i++) {
		while (tables[i].head != NULL) {
			tempP = tables[i].head;
			tables[i].head = tables[i].head->next;
			free(tempP);
		}
		// tables[i].head = NULL; can be deleted.
	}
	freeKitchen(&(rest->kitchen));
}

Bool isProductNameValid(char* name) {
	if (strlen(name) == 0 || strlen(name) > MAX_NAME_SIZE)
		return FALSE;
	return TRUE;
}

Bool isIntPositive(float floatToCheck) {
	if (floatToCheck < 0.0 || floor(floatToCheck) != ceil(floatToCheck)) // if negative or not int.
		return FALSE;
	return TRUE;
}

Bool isOperationValid(float op)
{
	// checks int op and its required boundries.
	if (op < 0.0 || op > 5.0 || floor(op) != ceil(op))
		return FALSE;
	return TRUE; // logical else
}

Bool checkTableNumber(float tableNumber)
{
	if (tableNumber < 0.0 || tableNumber > NUMBER_OF_TABLES || floor(tableNumber) != ceil(tableNumber))
		return FALSE;
	return TRUE;
}

/* Function checks if a name exists in list,
   if true returns the pointer to the product, if false returns NULL. */
/*pProduct getProductPtr(pList kitchen, char* productName)
{
	pProduct temp = kitchen->head;
	//checking for dish's instance in the kitchen
	while (temp != NULL) {
		if (!strcmp(temp->ProductName, productName))
			return temp;
		temp = temp->next;
	}
	return NULL;
} */