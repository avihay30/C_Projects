#include "Tree.h"

int main()
{
	treeData tree; // init tree.
	int operation = 0;
	int getNewKey, numOfKeysToPrint;
	Error err;
	Bool isKeyOccur;

	// init tree data.
	tree.numOfNodes = 0;
	tree.treeRoot = NULL;
	while (operation != END_PROGRAM) {
		printUserMenu();
		scanf("%d", &operation);
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
			isKeyOccur = isKeyExists(tree.treeRoot, getNewKey);
			if (isKeyOccur == FALSE) {
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
			break;

		default:
			puts(OPERATION_ERR);
		}
	}
	printf("\n\n---------------- Ty :) ----------------\n\n");
	return 0;
}
