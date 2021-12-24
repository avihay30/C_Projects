#include "Tree.h"

/** Adds new tree node if not exists, 
	else modifies the received error argument. */
void addTreeNode(pTreeNode *treeNode, int key, Error* error) {
	// Getting the address of appropriate pNode (double pointer).
	treeNode = findPNodePostion(treeNode, key);
	// Key doesn't exist.
	if (*treeNode == NULL) {
		pTreeNode temp;
		temp = createTreeNode(key);
		if (temp == NULL) {
			error->allocErr = TRUE;
			return;
		}
		// Logical else, If no error accrued while adding new node.
		*treeNode = temp;
	}
	// If key exists.
	else {
		error->keyAlreadyExist = TRUE;
	}
}

// Print tree inorder sequence.
void printInorder(pTreeData tData) {
	int numOfNodesInTree = tData->numOfNodes;
	printKNodesInorder(tData->treeRoot, &numOfNodesInTree);
}

// Returns the height of a tree.
int getTreeHeight(pTreeNode treeNode) {
	int lheight, rheight;
	if (treeNode == NULL)
		return -1;
	lheight = getTreeHeight(treeNode->left);
	rheight = getTreeHeight(treeNode->right);

	if (rheight > lheight)
		return rheight + 1;
	else
		return lheight + 1;
}

// Prints the max key in the tree.
void printMaxNodeKey(pTreeNode treeNode) {
	// The tree is empty.
	if (treeNode == NULL) {
		printf("The tree is empty.");
		return;
	}
	while (treeNode->right != NULL) {
		treeNode = treeNode->right;
	}

	printf("The max key in the tree is: %d\n", treeNode->key);
}

// Checking if key exists in the tree, returning appropriate Bool.
Bool isKeyExists(pTreeNode treeNode, int key) {
	pTreeNode* treeNodeAddress = findPNodePostion(&treeNode, key);
	
	// Returning whether the tree node exists or not.
	return *treeNodeAddress != NULL;
}

// Printing k keys of nodes in tree (inorder).
void printKNodesInorder(pTreeNode treeNode, int *k) {
	if (*k <= 0 || treeNode == NULL)
		return;

	printKNodesInorder(treeNode->left, k);
	*k = *k - 1;
	// In case the left branch is smaller than k.
	if (*k < 0)
		return;
	printf("%d -> ", treeNode->key);
	printKNodesInorder(treeNode->right, k);
}

// Returning an empty malloced node.
treeNode* createTreeNode(int key) {
	treeNode* newNode;
	newNode = (treeNode*)malloc(sizeof(treeNode));
	if (newNode != NULL) {
		newNode->key = key;
		newNode->left = NULL;
		newNode->right = NULL;
	}
	return newNode;
}

/** Searching for key in the tree. 
	If exists return the address of the pNode that contains the key.
	else, return the appropriate address to place the not existed node. */
pTreeNode* findPNodePostion(pTreeNode *treeNode, int key) {
	// Arrived to existing node or the appropriate position of the node.
	if (*treeNode == NULL || (*treeNode)->key == key)
		return treeNode;

	if ((*treeNode)->key > key)
		return findPNodePostion(&(*treeNode)->left, key);
	else
		return findPNodePostion(&(*treeNode)->right, key);
}

/** Function get pointer to a restaurant struct
	and frees all allocated memory. */
void freeAll(pTreeNode root)
{
	if (root != NULL) {
		freeAll(root->left);
		freeAll(root->right);
		free(root);
	}
}

// Function to get an int input variable 
void inputInt(char* inputMsg, char* inputMsgErr, int* inputtedKey) {
	Bool isValid;
	int numOfInputs;
	do {
		isValid = TRUE;
		printf(INPUT_NEW_INT, inputMsg);
		numOfInputs = scanf("%d", inputtedKey);
		if(numOfInputs != VALID_INPUT) {
			printf(NEW_INT_INPUT_ERR, inputMsgErr);
			isValid = FALSE;
			rewind(stdin);
		}
	} while (isValid != TRUE);
}

/* Function gets a dummyChoice(float pointer) and int pointer
   and cast the float pointer to int only if user inputted a valid input.*/
void getOperation(double* fOperation, int* operation) {
	printf("\tYour choice: ");
	while (TRUE) {
		// getting input and checking if it's int(by floor and ceil) and it's boundaries[0,7].
		if (scanf("%lf", fOperation) && floor(*fOperation) == ceil(*fOperation) && (*fOperation > 0.0 && *fOperation <= 7.0)) {
			*operation = (int)*fOperation; // user entered int (fchoice is an 'int', like 2.0).
			break;
		}
		printf("\tInvalid input! please enter *int* between 1<->7: "); // logical else
		rewind(stdin); // in case user inputs string.
	}
	puts("\n");
}


void initTreeDate(treeData* pTreeData) {
	pTreeData->treeRoot = NULL;
	pTreeData->numOfNodes = 0;
}

void initError(Error* err) {
	err->allocErr = FALSE;
	err->keyAlreadyExist = FALSE;
}

void printUserMenu() {
	printf("\n\tPlease choose an action:\n");
	printf("\t-> 1: Add a new key.\n");
	printf("\t-> 2: Print keys by Inorder.\n");
	printf("\t-> 3: Print tree height.\n");
	printf("\t-> 4: Print tree max key.\n");
	printf("\t-> 5: Find key.\n");
	printf("\t-> 6: Print k keys by Inorder.\n");
	printf("\t-> 7: End program.\n");
	printf("\t```````````````````````````````````````````````````````\n");
}
