#include "defs.h"

// Defines the following definitions, if it hasn't been defined
#ifndef SYNTAXTREE__
#define SYNTAXTREE__

/*
 * <_CONTENT Union>
 * Union for storing actual data from expression
 */
typedef union {
	double data;	// Number Literal
	char operation;	// Operator itself
	int index;		// Index of variable in Symbol Talbe
}_CONTENT;

/*
 * <NODE Struct>
 * Struct for node of syntax tree (as a binary tree)
 */
typedef struct node {
	struct node *left;	// Pointer to Left child of Node
	struct node *right;	// Pointer to Right child of Node
	short type;			// Flag Variable to distinguish which data is stored in _CONTETN
	_CONTENT content;	// Actual Data from expression
}NODE;

typedef struct tree {
	NODE* statement;
	struct tree *next;
	struct tree *falseNext;
}TREE;

// Following definitions are for functions from syntaxTree.c
void nodeAssignment(int index);
void nodeNum(double val);
void nodeVariable(int index);
void nodeUnaryMinus();

void nodePlus();
void nodeMinus();
void nodeMultiply();
void nodeDivide();

void nodeGreater(int equal);
void nodeLess(int equal);
void nodeEqual();
void nodeNotEqual();

extern void initializeRoot();
NODE* createDataNode(double data);
NODE* createOperatorNode(char operation);
NODE* createStringNode(int index);
NODE* createControlNode(char type);
void pushStack(NODE* data);
NODE* popStack();
NODE* stackReduce(char operation);
extern void traverseRoot();
void traverseTree(NODE* node, int i, int j);
void freeTree(NODE* node);

void treeLine();
void treeIf();
void treeElse();
void treeWhile();
void traverseBlock();
void insertTree(NODE* new);
void freeBlock(TREE* tree);
#endif // !SYNTAXTREE__