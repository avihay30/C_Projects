#include "Restaurant.h"

// Function addes products to the *end* of the list
void CreateProducts(pList prodList)
{
	FILE* f;
	float quantity, price;
	char dummyName[MAX_NAME_SIZE + 100]; // for testing.
	pProduct tempProd;
	f = fopen("Manot.txt", "r");
	checkAllocation(f, FILE_ERR, prodList, freeKitchen);
	while (fscanf(f, "%s %f %f", dummyName, &quantity, &price) != EOF) {
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
		// building product [all inputs from file are valid casting is safe]
		tempProd = (pProduct)malloc(sizeof(product));
		checkAllocation(tempProd, ALLOC_ERR, prodList, freeKitchen);
		tempProd->ProductName = (char*)malloc(strlen(dummyName) + 1);
		checkAllocation(tempProd->ProductName, ALLOC_ERR, prodList, freeKitchen);
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

void AddItems(pList prodList, char* productName, float fQuantity)
{
	pProduct productPtr;
	productPtr = getProductPtr(prodList, productName);
	if(productPtr == NULL){
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

void OrderItem(pRestaurant rest, float fTableNumber, char* productName, float fQuantity)
{
	int tableNumber, quantity;
	pProduct productPtr;
	pOrder currentOrder;

	if(!checkTableNumber(fTableNumber)) { // table number validation
		printf(INVALID_TABLE_NUM);
		return;
	}

	tableNumber = (int)fTableNumber;
	productPtr = getProductPtr(&(rest->kitchen), productName);
	if(productPtr == NULL) { // if name doen't exists in kitchen.
		printf(PRODUCT_UNAVILEBLE, productName);
		return;
	}
	if (!isIntPositive(fQuantity) && (int)fQuantity > productPtr->Quantity) { // quantity number validation
		printf("Invalid given product quantity!\n");
		return;
	}
	quantity = (int)fQuantity;

	// building node
	currentOrder = (pOrder)malloc(sizeof(order));
	checkAllocation(currentOrder, ALLOC_ERR, rest, freeAll);
	currentOrder->Quantity = quantity;
	// having pointer to product in kitchen in order to save redundent malloc to prodName again.
	// and for calculation of price in "RemoveTable" function below.
	currentOrder->product = productPtr;
	currentOrder->next = NULL;

	// inserting to head of list
	currentOrder->next = rest->tables[tableNumber].head;
	rest->tables[tableNumber].head = currentOrder;
	productPtr->Quantity -= quantity;
	printf(ADD_TO_TABLE, quantity, productName, tableNumber);
}

void RemoveItem(orders tables[], float fTableNumber)
{
	pOrder orderToDelete;
	int tableNumber;
	if(!checkTableNumber(fTableNumber)){
		printf(INVALID_TABLE_NUM);
		return;
	}
	tableNumber = (int)fTableNumber;
	if(tables[tableNumber].isCanceledOrder){
		printf("Table number %d : has used the remove function once!\n", tableNumber);
		return;
	}
	if(tables[tableNumber].head == NULL){
		printf(TABLE_DIDNT_ORDERD, tableNumber);
		return;
	}
	orderToDelete = tables[tableNumber].head;
	tables[tableNumber].head = orderToDelete->next;
	printf(PRODUCT_RETURNED, orderToDelete->Quantity, orderToDelete->product->ProductName, tableNumber);
	free(orderToDelete);
	tables[tableNumber].isCanceledOrder = TRUE;
}

void RemoveTable(orders tables[], float fTableNumber)
{
	pOrder orderToDelete;
	int tableNumber, priceToPay = 0;
	float tipToPay;
	if(!checkTableNumber(fTableNumber)){
		printf(INVALID_TABLE_NUM);
		return;
	}
	tableNumber = (int)fTableNumber;
	if(tables[tableNumber].head == NULL){
		printf(TABLE_DIDNT_ORDERD, tableNumber);
		return;
	}
	orderToDelete = tables[tableNumber].head;
	while(orderToDelete != NULL){
		printf("%d %s, ", orderToDelete->Quantity, orderToDelete->product->ProductName);
		priceToPay += orderToDelete->product->Price * orderToDelete->Quantity;
		tables[tableNumber].head = tables[tableNumber].head->next;
		free(orderToDelete);
		orderToDelete = tables[tableNumber].head;
	}
	tipToPay = (float)priceToPay * 0.15;
	printf(PRICE, priceToPay, (int)tipToPay); // todo: check if tip is int
	tables[tableNumber].head = NULL;
}

// _-'-_ Util functions _-'-_

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
	prodList->tail = NULL; // reseting to initial value[head already = NULL].
}

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