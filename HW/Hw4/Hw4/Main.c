#include "Restaurant.h"

int main()
{
	FILE* f;
	restaurant rest; // init restaurant.
	int i;
	float fOperation, tableNum, quantity;
	int operation, numOfInputs;
	char prodName[MAX_NAME_SIZE + 100];

	// reseting rest list.
	for (i = 0; i < NUMBER_OF_TABLES; i++) { // setting restaurent tables.
		rest.tables[i].head = NULL;
		rest.tables[i].isCanceledOrder = FALSE;
	}
	rest.kitchen.head = rest.kitchen.tail = NULL;

	f = fopen("Instructions.txt", "rt");
	checkAllocation(f, FILE_ERR, &rest, freeAll);
	// reading Instructions.txt and call the appropriate functions.
	while ((numOfInputs = fscanf(f, "%f", &fOperation)) != EOF) {
		if (numOfInputs != 1 || !isOperationValid(fOperation)) // checking instructions.txt
			checkAllocation(NULL, FILE_READ_ERR, &rest, freeAll);
		operation = (int)fOperation; // operation is valid.

		// if first run and operation isn't 1.
		if (rest.kitchen.head == NULL && operation != 1) {
			printf(INSTRUCTIONS_ERR);
			exit(1); // no need to free.
		}

		// if operation 1 happends more than once.
		if (rest.kitchen.head != NULL && operation == 1) {
			printf(INSTRUCTIONS_ERR);
			printf("Operation 1 cannot be happend twice!\n");
			continue;
		}
		// call the appropriate functions
		switch (operation) {
		case 1:
			CreateProducts(&(rest.kitchen));
			break;
		case 2:
			fscanf(f, "%s %f", prodName, &quantity); // assume file valid(as noted)
			AddItems(&(rest.kitchen), prodName, quantity);
			break;
		case 3:
			fscanf(f, "%f %s %f", &tableNum, prodName, &quantity); // assume file valid(as noted)
			OrderItem(&rest, tableNum, prodName, quantity);
			break;
		case 4:
			fscanf(f, "%f", &tableNum); // assume file valid(as noted)
			RemoveItem(rest.tables, tableNum);
			break;
		case 5:
			fscanf(f, "%f", &tableNum); // assume file valid(as noted)
			RemoveTable(rest.tables, tableNum);
			break;
		}
	}
	freeAll(&rest);
	return 0;
}