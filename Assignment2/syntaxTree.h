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
	char operator;	// Operator itself
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

// Following definitions are for functions from syntaxTree.c
void nodeAssignment();
void nodeNum(double val);
int nodeVariable(int index, int tok);
void nodeUnaryMinus();

void nodePlus();
void nodeMinus();
void nodeMultiply();
void nodeDivide();

void initializeRoot();
NODE* createDataNode(double data);
NODE* createOperatorNode(char operator);
NODE* createStringNode(int index);
void pushStack(NODE* data);
NODE* popStack();
NODE* stackReduce(char operator);

void traverseTree(NODE* node, int i, int j);
void freeTree(NODE* node);
#endif // !SYNTAXTREE__