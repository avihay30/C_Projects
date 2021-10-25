#include "Tree.h"

// Adds new tree node if not exists, else returns error.
void addTreeNode(pTreeNode *treeNode, int key, Error* error) {
	treeNode = findPNodePostion(treeNode, key);
	// Key not exists.
	if (*treeNode == NULL) {
		pTreeNode temp;
		temp = createTreeNode(key);
		if (temp == NULL) {
			error->allocErr = TRUE;
			return;
		}
		// logical else, If no error accured while adding node.
		*treeNode = temp;
	}		
	else {
		error->keyAlreadyExist = TRUE;
	}
}

// Print tree inorder sequnce.
void printInorder(pTreeData tData) {
	int numOfNudesInTree = tData->numOfNodes;
	printKNodesInorder(tData->treeRoot, &numOfNudesInTree);
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
	}
	while (treeNode->right != NULL) {
		treeNode = treeNode->right;
	}

	printf("The max key in the tree is: %d", treeNode->key);
}

Bool isKeyExists(pTreeNode treeNode, int key) {
	pTreeNode* treeNodeAddress = findPNodePostion(&treeNode, key);
	
	// Returning whether the tree node exists or not.
	return *treeNodeAddress != NULL;
}

void printKNodesInorder(pTreeNode treeNode, int *k) {
	if (k == 0 || treeNode == NULL)
		return;

	printKNodesInorder(treeNode->left, k);
	*k = *k - 1;
	printf("%d -> ", treeNode->key);
	printKNodesInorder(treeNode->right, k);
}

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
	If exists return the address of the node that contains the key.
	else, return the appropriate address to place the not existed node. */
pTreeNode* findPNodePostion(pTreeNode *treeNode, int key) {
	// Arrived to exsiting node or the appropriate position of the node.
	if (*treeNode == NULL || (*treeNode)->key == key)
		return treeNode;

	if ((*treeNode)->key > key)
		return findPNodePostion(&(*treeNode)->left, key);
	else
		return findPNodePostion(&(*treeNode)->right, key);
}

/* 
	Function get pointer to a restaurant struct
	and frees all allocated memory. 
 */
void freeAll(pTreeNode root)
{
	if (root != NULL){
		freeAll(root->left);
		freeAll(root->right);
		free(root);
	}
	root = NULL;
}

// Function to input an int variable 
void inputInt(const char* inputMsg, const char* inputMsgErr, int* key){
	Bool isValid = TRUE;
	int numOfInputs;
	do {
		printf(INPUT_NEW_INT, inputMsg);
		numOfInputs = scanf("%d", key);
		if(numOfInputs != VALID_INPUT){
			printf(NEW_INT_INPUT_ERR, inputMsgErr);
			isValid = FALSE;
		}
	} while (isValid != TRUE);
}

void initError(Error* err) {
	err->allocErr = FALSE;
	err->keyAlreadyExist = FALSE;
}

void printUserMenu(){
	printf("\n\tPlease choose an action:\n");
	printf("\t-> 1: Add a new key.\n");
	printf("\t-> 2: Print keys by Inorder.\n");
	printf("\t-> 3: Print tree height.\n");
	printf("\t-> 4: Print tree max key.\n");
	printf("\t-> 5: Find key.\n");
	printf("\t-> 6: Print k keys by Inorder.\n");
	printf("\t-> 7: End program.\n");
	printf("\t```````````````````````````````````````````````````````\n->: ");
}
