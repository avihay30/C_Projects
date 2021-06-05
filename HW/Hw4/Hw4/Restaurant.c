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
		
		// inserting to end of list
		if (prodList->head == NULL)
			prodList->head = tempProd;
		else
			prodList->tail->next = tempProd;
		prodList->tail = tempProd;
	}
	fclose(f);
	printf(KITCHEN_CREATED);
}

void AddItems(pList prodList, char* productName, float quantity)
{
	pProduct productPtr;
	productPtr = getProductPtr(prodList ,productName);
	if(productPtr == NULL){
		printf("%s does not exist in the kitchen!\n",productName);
		return;
	}
	if (quantity < 0.0 || floor(quantity) != ceil(quantity)){
		printf("Invalid given product quantity!\n");
		return;
	}
	productPtr->Quantity += (int)quantity;
	printf(ADD_TO_KITCHEN, (int)quantity, productName);
}

void OrderItem(pRestaurant rest, float fTableNumber, char* productName, float quantity)
{
	pProduct productPtr;
	pOrder putAnOrder;
	pOrder tableOrders;
	int tableNumber;
	if(!checkTableNumber(fTableNumber)){
		printf(INVALID_TABLE_NUM);
		return;
	}
	tableNumber = (int)fTableNumber;
	productPtr = getProductPtr(&(rest->kitchen) ,productName);
	if(productPtr == NULL){
		printf(PRODUCT_UNAVILEBLE,productName);
		return;
	}
	if(!isIntPositive(quantity) && (int)quantity > productPtr->Quantity){
		printf("Invalid given product quantity!\n");
		return;
	}
	productPtr->Quantity -= (int)quantity;
	putAnOrder = (pOrder)malloc(sizeof(order));
	checkAllocation(putAnOrder, ALLOC_ERR, rest, freeAll);
	putAnOrder->Quantity = (int)quantity;
	putAnOrder->product = productPtr;
	putAnOrder->next = NULL;
	tableOrders = rest->tables[tableNumber].head;
	if (tableOrders == NULL)
		rest->tables[tableNumber].head = putAnOrder;
	else{
		putAnOrder->next = tableOrders;
		rest->tables[tableNumber].head = putAnOrder;
	}
	printf(ADD_TO_TABLE, (int)quantity, productName, (int)tableNumber);
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
	int priceToPay = 0;
	float tipToPay;
	int tableNumber;
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
	tipToPay = priceToPay * (float)0.15;
	printf(PRICE, priceToPay, (int)tipToPay);
	tables[tableNumber].head = NULL;
}