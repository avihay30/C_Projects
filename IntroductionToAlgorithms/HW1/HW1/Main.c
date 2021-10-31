#include "Tree.h"

int main()
{
	int operation = 0;
	int getNewKey, numOfKeysToPrint;
	double dummyOperation = 0;
	// Init tree
	treeData tree;
	Error err;
	Bool isKeyInTree;

	// Init tree data.
	initTreeDate(&tree);
	while (operation != END_PROGRAM) {
		printUserMenu();
		getOperation(&dummyOperation, &operation);

		// call the appropriate functions
		switch (operation) {
		case ADD_NEW_NODE:
			initError(&err);
			inputInt("a new key", "key", &getNewKey);
			addTreeNode(&(tree.treeRoot), getNewKey, &err);
			if (err.allocErr) {
				puts(ALLOC_ERR);
				operation = END_PROGRAM;
			}
			else if (err.keyAlreadyExist) {
				puts(KEY_ALREADY_EXISTS_ERR);
			}
			else {
				tree.numOfNodes++;
			}
			break;

		case PRINT_KEYS_BY_INORDER:
			printf("Print keys by Inorder ->\n");
			printInorder(&tree);
			printf("\n\n");
			break;

		case GET_TREE_HEIGHT:
			printf("The tree height is : %d\n", getTreeHeight(tree.treeRoot));
			break;

		case PRINT_MAX_KEY:
			printMaxNodeKey(tree.treeRoot);
			break;

		case FIND_KEY:
			inputInt("key to search", "key", &getNewKey);
			isKeyInTree = isKeyExists(tree.treeRoot, getNewKey);
			if (isKeyInTree == FALSE) {
				printf(KEY_DOESNT_EXISTS_ERR, getNewKey);
			}
			else {
				printf(KEY_EXISTS, getNewKey);
			}
			break;

		case PRINT_K_KEYS_BY_INORDER:
			inputInt("amount of keys to print", "amount", &numOfKeysToPrint);
			while (numOfKeysToPrint > tree.numOfNodes) {
				inputInt(PRINT_K_KEYS_ERR, "amount", &numOfKeysToPrint);
			}
			printf("Printing %d keys by Inorder ->\n", numOfKeysToPrint);
			printKNodesInorder(tree.treeRoot, &numOfKeysToPrint);
			printf("\n\n");
			break;

		case END_PROGRAM:
			freeAll(tree.treeRoot);
			initTreeDate(&tree);
			break;

		default:
			puts(OPERATION_ERR);
		}
	}
	printf("\n\n---------------- Ty :) ----------------\n\n");
	return 0;
}
