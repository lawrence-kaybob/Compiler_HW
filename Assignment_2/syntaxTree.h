#include "defs.h"

#ifndef SYNTAXTREE__
#define SYNTAXTREE__
typedef union {
	double data;
	char operator;
	int index;
}_CONTENT;

typedef struct node {
	struct node *left;
	struct node *right;
	short type;
	_CONTENT content;
}NODE;

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