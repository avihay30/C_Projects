#ifndef _Tree
#define _Tree

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define Declerations.
#define ALLOC_ERR "AlloctionError: The program couldn't allocate memory!\n"
#define KEY_ALREADY_EXISTS_ERR "AddingNodeError: Couldn't add the node, node already exists!\n"
#define KEY_DOESNT_EXISTS_ERR "IsKeyExist: Couldn't find the key %d!\n"
#define KEY_EXISTS "IsKeyExist: The key %d was found!\n"
#define OPERATION_ERR "OperationError: Given operation isn't valid, Please try again!\n"
#define PRINT_K_KEYS_ERR "PrintKKeysError: Given k keys to print is invalid (maybe too large), Please try again!\n"
#define INPUT_NEW_INT "Please enter %s ->: "
#define NEW_INT_INPUT_ERR "\nWrong %s, Please try again!"

#define EQUALS  0
#define VALID_INPUT 1

#define ADD_NEW_NODE  1
#define PRINT_KEYS_BY_INORDER  2
#define GET_TREE_HEIGHT 3
#define PRINT_MAX_KEY 4
#define FIND_KEY 5
#define PRINT_K_KEYS_BY_INORDER 6
#define END_PROGRAM  7

// Struct Declerations. 
typedef enum { FALSE, TRUE } Bool;

// define tree struct
typedef struct treeNode
{
	struct treeNode *right, *left;
	int key;
}treeNode,* pTreeNode;

typedef struct treeData
{
	pTreeNode treeRoot;
	int numOfNodes;
}treeData, * pTreeData;

typedef struct Error
{
	Bool allocErr;
	Bool keyAlreadyExist;
}Error;

void addTreeNode(pTreeNode*, int, Error*);
void printInorder(pTreeData);
int getTreeHeight(pTreeNode);
void printMaxNodeKey(pTreeNode);
Bool isKeyExists(pTreeNode, int);
void printKNodesInorder(pTreeNode, int*);
treeNode* createTreeNode(int);
pTreeNode* findPNodePostion(pTreeNode*, int);
void freeAll(pTreeNode);
void inputInt(const char* , const char* , int*);
void printUserMenu();
void initError(Error*);

#endif // _Tree
