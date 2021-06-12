#include "Restaurant.h"

/* Function gets products list 
   and adds products to the *end* of the list from Manot.txt file */
void CreateProducts(pList prodList)
{
	FILE* f;
	float quantity, price;
	char dummyName[MAX_NAME_SIZE + 100]; // for testing.
	pProduct tempProd;
	f = fopen("Manot.txt", "r");
	checkAllocation(f, FILE_ERR, prodList, freeKitchen);
	// reading Manot file and inserting to 'prodList'
	while (fscanf(f, "%s %f %f", dummyName, &quantity, &price) != EOF) {
		// check input validations.
		if (!isProductNameValid(dummyName)) {
			printf("product name - %s is not in the up the required size %d", dummyName, MAX_NAME_SIZE);
			continue;
		}
		if (!isIntPositive(quantity)) {
			printf("Invalid given product quantity!(%f)\n", quantity);
			continue;
		}
		if (!isIntPositive(price)) {
			printf("Invalid given product price!(%f)\n", price);
			continue;
		}
		if (getProductPtr(prodList, dummyName) != NULL) { // if product name exits.
			printf(NAME_DUPL_ERR, dummyName);
			continue;
		}
		// building product [all inputs from file are valid and casting is safe]
		tempProd = (pProduct)malloc(sizeof(product));
		checkAllocation(tempProd, ALLOC_ERR, prodList, freeKitchen);
		tempProd->ProductName = (char*)malloc(strlen(dummyName) + 1);
		checkAllocation(tempProd->ProductName, ALLOC_ERR, prodList, freeKitchen);
		// filling tempProd [node].
		strcpy(tempProd->ProductName, dummyName);
		tempProd->Quantity = (int)quantity;
		tempProd->Price = (int)price;
		tempProd->next = NULL;
		
		// inserting to tail of list
		if (prodList->head == NULL)
			prodList->head = tempProd;
		else
			prodList->tail->next = tempProd;
		prodList->tail = tempProd;
	}
	fclose(f);
	printf(KITCHEN_CREATED);
}

/* Function gets prodList and prodName and quantity and adds the 
   quantity to an existing product in kitchen */
void AddItems(pList prodList, char* productName, float fQuantity)
{
	pProduct productPtr;
	productPtr = getProductPtr(prodList, productName);
	if (productPtr == NULL) {
		printf("%s does not exist in the kitchen!\n", productName);
		return;
	}
	if (!isIntPositive(fQuantity)) {
		printf("Invalid given product quantity!\n");
		return;
	}
	productPtr->Quantity += (int)fQuantity;
	printf(ADD_TO_KITCHEN, (int)fQuantity, productName);
}

/* Function gets restaurant list, tableNumber and an item to order and adds the
   order to *head* of orders in the table */
void OrderItem(pRestaurant rest, float fTableNumber, char* productName, float fQuantity)
{
	int tableIndex, quantity;
	pProduct productPtr;
	pOrder currentOrder;

	if (!checkTableNumber(fTableNumber)) { // table number validation.
		printf(INVALID_TABLE_NUM);
		return;
	}

	tableIndex = (int)fTableNumber - 1;
	productPtr = getProductPtr(&(rest->kitchen), productName);
	if (productPtr == NULL) { // if name doesn't exists in kitchen.
		printf(PRODUCT_UNAVILEBLE, productName);
		return;
	}
	// quantity number validation
	if (!isIntPositive(fQuantity) && (int)fQuantity > productPtr->Quantity) {
		printf("Invalid given product quantity!\n");
		return;
	}
	quantity = (int)fQuantity;

	// building node
	currentOrder = (pOrder)malloc(sizeof(order));
	checkAllocation(currentOrder, ALLOC_ERR, rest, freeAll);
	// filling order [node].
	currentOrder->Quantity = quantity;
	// having pointer to product in kitchen in order to save redundent malloc to prodName again.
	// and for calculation of price in "RemoveTable" function below.
	currentOrder->product = productPtr;
	currentOrder->next = NULL;

	// inserting to head of list
	currentOrder->next = rest->tables[tableIndex].head;
	rest->tables[tableIndex].head = currentOrder;

	productPtr->Quantity -= quantity; // decremeting quantity in kitchen.
	printf(ADD_TO_TABLE, quantity, productName, tableIndex + 1);
}

/* Function gets an array of tables and a table number and
   removes the last order of the table (if exists) */
void RemoveItem(orders tables[], float fTableNumber)
{
	pOrder orderToDelete;
	int tableIndex;
	if (!checkTableNumber(fTableNumber)) { // table number validation.
		printf(INVALID_TABLE_NUM);
		return;
	}
	tableIndex = (int)fTableNumber - 1;
	if (tables[tableIndex].head == NULL) { // checking if table ordered.
		printf(TABLE_DIDNT_ORDERED, tableIndex + 1);
		return;
	}
	// checking if table already canceled.
	if (tables[tableIndex].isCanceledOrder) {
		printf("Table number %d : has used the remove function once!\n", tableIndex + 1);
		return;
	}
	// moving an free head node (deleting order).
	orderToDelete = tables[tableIndex].head;
	tables[tableIndex].head = orderToDelete->next;
	printf(PRODUCT_RETURNED, orderToDelete->Quantity, orderToDelete->product->ProductName, tableIndex + 1);
	free(orderToDelete);
	tables[tableIndex].isCanceledOrder = TRUE;
}

/* Function gets an array of tables and table number
   and remove (free) all memory of orders of the table, and prints bill. */
void RemoveTable(orders tables[], float fTableNumber)
{
	pOrder orderToDelete;
	int tableIndex, priceToPay = 0;
	float tipToPay;
	if (!checkTableNumber(fTableNumber)) { // table number validation.
		printf(INVALID_TABLE_NUM);
		return;
	}
	tableIndex = (int)fTableNumber - 1;
	if (tables[tableIndex].head == NULL) { // checking if table ordered.
		printf(TABLE_DIDNT_ORDERED, tableIndex + 1);
		return;
	}
	// deleting (free) orders of tables.
	orderToDelete = tables[tableIndex].head;
	while (orderToDelete != NULL) {
		printf("%d %s, ", orderToDelete->Quantity, orderToDelete->product->ProductName);
		priceToPay += orderToDelete->product->Price * orderToDelete->Quantity;
		tables[tableIndex].head = tables[tableIndex].head->next;
		free(orderToDelete);
		orderToDelete = tables[tableIndex].head;
	}
	tipToPay = (float)priceToPay * 0.15;
	printf(PRICE, priceToPay, tipToPay);
	tables[tableIndex].head = NULL; // reseting head to null.
}

// _-'-_ Util functions _-'-_

// Function checks if pointer allocation is invalid and free memory needed and exit. 
void checkAllocation(void* pToCheck, char* message, void* listToFree, void(*fp)(void*))
{
	if (pToCheck == NULL) {
		fprintf(stderr, "%s\n", message);
		fp(listToFree); // call the appropriate free(fp) function.
		exit(1); // closes file automaticaly.
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
	prodList->tail = NULL; // reseting to initial value[head already = NULL].
}

/* Function get pointer to a restaurant struct
   and frees all allocated memory. */
void freeAll(pRestaurant rest)
{
	int i;
	pOrders tables = rest->tables;
	pOrder tempP;
	for (i = 0; i < NUMBER_OF_TABLES; i++) {
		while (tables[i].head != NULL) {
			tempP = tables[i].head;
			tables[i].head = tables[i].head->next;
			free(tempP);
		}
	}
	freeKitchen(&(rest->kitchen));
}

// Function gets a string of a product name and returns whether it's valid.
Bool isProductNameValid(char* name) {
	if (strlen(name) == 0 || strlen(name) > MAX_NAME_SIZE)
		return FALSE;
	return TRUE;
}

// Function gets a float and returns whether it's a positive int.
Bool isIntPositive(float floatToCheck) {
	// if negative or not int.
	if (floatToCheck <= 0.0 || floor(floatToCheck) != ceil(floatToCheck))
		return FALSE;
	return TRUE;
}

// Function gets a float of an operation and returns whether it's valid[1,5].
Bool isOperationValid(float op)
{
	// checks int op and its required boundaries.
	if (op < 1.0 || op > 5.0 || floor(op) != ceil(op))
		return FALSE;
	return TRUE; // logical else
}

// Function gets a float of an tableNumber and returns whether it's valid[1,50].
Bool checkTableNumber(float tableNumber)
{
	if (tableNumber < 1.0 || tableNumber > NUMBER_OF_TABLES || floor(tableNumber) != ceil(tableNumber))
		return FALSE;
	return TRUE;
}

/* Function checks if a name exists in list,
   if true returns the pointer to the product, if false returns NULL. */
pProduct getProductPtr(pList kitchen, char* productName)
{
	pProduct temp = kitchen->head;
	//checking for product instance in the kitchen
	while (temp != NULL) {
		if (!strcmp(temp->ProductName, productName))
			return temp;
		temp = temp->next;
	}
	return NULL;
}