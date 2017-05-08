#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "syntaxTree.h"
#include "defs.h"

NODE* root;
NODE* stack[20];
int stackIndex = 0;

extern char varName[100][100];

double varValue[100] = { 0 };
extern int entryNo;
int localEntryNo = 0;

void traverseTree(NODE* node, int i, int j);
void freeTree(NODE* node);

void nodeAssignment() {
	NODE* new;
	NODE* variable;

	new = popStack();
	variable = popStack();
	pushStack(variable);
	pushStack(new);
	new = stackReduce('=');
	pushStack(new);

	entryNo = localEntryNo;
}

void nodeNum(double val) {
	NODE* new = createDataNode(val);
	pushStack(new);
}

int nodeVariable(int index, int tok) {
	NODE* new;
	if (index >= entryNo) {
		if (tok != ASSIGN) {
			fprintf(stderr, "Variable Not Defined\n");
			return  ERROR;
		}
	}

	new = createStringNode(index);
	pushStack(new);
	return index;
	
}

void nodeUnaryMinus() {
	NODE* data = createDataNode(0.0);
	NODE* new = popStack();

	pushStack(data);
	pushStack(new);
	new = stackReduce('-');
	pushStack(new);
	// return -val;
}

void nodePlus() {
	NODE* new = stackReduce('+');
	pushStack(new);
}

void nodeMinus() {
	NODE* new = stackReduce('-');
	pushStack(new);
}

void nodeMultiply() {
	NODE* new = stackReduce('*');
	pushStack(new);
}

void nodeDivide() {
	NODE* new = stackReduce('/');
	pushStack(new);
}

void initializeRoot() {
	root = (NODE*)malloc(sizeof(NODE));
	root->left = NULL;
	root->right = NULL;
	stackIndex = 0;
}

NODE* createDataNode(double data) {
	NODE* new = (NODE*)malloc(sizeof(NODE));
	new->left = NULL;
	new->right = NULL;
	new->content.data = data;
	new->type = DATA;

	root = new;

	return new;
}

NODE* createOperatorNode(char operator) {
	NODE* new = (NODE*)malloc(sizeof(NODE));
	new->left = NULL;
	new->right = NULL;
	new->content.operator = operator;
	new->type = OPERATOR;

	return new;
}

NODE* createStringNode(int index) {
	NODE* new = (NODE*)malloc(sizeof(NODE));
	new->left = NULL;
	new->right = NULL;
	new->content.index = index;
	new->type = VARIABLE;

	root = new;

	return new;
}

void pushStack(NODE* data) {
	stack[stackIndex++] = data;
}

NODE* popStack() {
	return stack[--stackIndex];
}

NODE* stackReduce(char operator) {
	NODE* new = createOperatorNode(operator);
	new->right = popStack();
	new->left = popStack();

	root = new;

	return new;
}

void traverseTree(NODE* node, int i, int j) {
	if (node->type == DATA)
		printf("(%d,%d)\t%f\n", i, j, node->content.data);
	else if (node->type == OPERATOR)
		printf("(%d,%d)\t%c\n", i, j, node->content.operator);
	else
		printf("(%d,%d)\t%s\n", i, j, varName[node->content.index]);

	if (node->left != NULL)
		traverseTree(node->left, i + 1, j);

	if (node->right != NULL)
		traverseTree(node->right, i + 1, j + 1);
}

void freeTree(NODE* node) {
	if (node->left != NULL) {
		freeTree(node->left);
		free(node->left);
	}
	if (node->right != NULL) {
		freeTree(node->right);
		free(node->right);
	}
}