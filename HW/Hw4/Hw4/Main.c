#include "Restaurant.h"

int main()
{
	FILE* f;
	list productsList; // init list.
	float foperation;
	int operation, numOfInputs;

	productsList.head = productsList.tail = NULL;
	productsList.size = 0;
	f = fopen("Instructions.txt", "rt");
	checkAllocation(f, FILE_ERR, &productsList);
	while ((numOfInputs = fscanf(f, "%f", &foperation)) != EOF) {
		if (numOfInputs != 1 || !isOperationValid(foperation))
			checkAllocation(NULL, FILE_READ_ERR, &productsList);
		operation = (int)foperation; // operation is valid.

		// if first run and operation isn't 1.
		if (productsList.head == NULL && operation != 1) {
			printf(INSTRUCTIONS_ERR);
			exit(1); // no need to free.
		}

		switch (operation) {
		case 1:
			CreateProducts(&productsList);
		}
	}
	return 0;
}