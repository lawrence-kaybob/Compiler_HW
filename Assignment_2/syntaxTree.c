#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "syntaxTree.h"
#include "defs.h"

/*
 * <Syntax Tree Related Variables>
 * Following variables are essential for constructing
 * syntax tree of an expression
 */
NODE* root;			// Root node of syntax tree
NODE* stack[20];		// Temporary stack for nodes to be constructed
int stackIndex = 0;		// Trailer variable of stack

/*
 * <Symbol Table Related Variable>
 * Following variables are essential for recognizing
 * variable name & its value
 */

// External Variable imported from equationParser.l or equationParser.flex.cpp
extern char varName[100][100];	// Symbol Table for variable name
extern int entryNo;		// Trailer variable for symbol table

double varValue[100] = { 0 };	// Value Table for variables in symbol Table
int localEntryNo = 0;		// Temporary entryNo variable for assigning new variables

// Pre-definition for some fucntions
void traverseTree(NODE* node, int i, int j);
void freeTree(NODE* node);

/**
 * <nodeAssignemnt()>
 * Binds two nodes with '='(Assign) operator.
 * Had variable to be left of the operator
 * i.e.) a=2 -->	'='
 *			/ \
 *			a   2
 */
void nodeAssignment() {
	NODE* new;
	NODE* variable;
					// Because of right-associativity of '='(assign) opreator,
	new = popStack();		// the value to be assigned is pushed earlier than the variable.
	variable = popStack();		// Therefore, it pops variable and number(or expression) node.
	pushStack(variable);		// then, it pushes variable node first,
	pushStack(new);			// number node later.
	new = stackReduce('=');		// This will contruct syntax tree for assignment statement,
	pushStack(new);			// as it does for other operators

	entryNo = localEntryNo;		// This updates the symbole table trailer variable, when new variable initlaized.
}

/**
 * <nodeNUm(double val)>
 * - Input : Number literal to be node-ify
 * Creates a node for number literal when identified.
 * Then, pushed to stack, for further operators.
 */
void nodeNum(double val) {
	NODE* new = createDataNode(val);
	pushStack(new);
}

/**
 * <nodeVariable(int index)
 * - Input
 * 		- index : index of variable in symbol table
 *		- tok : Lookahead token for variable, determining it is assign statement.
 * Creates a node for identifier.
 * Then, pushed to stack, for further operators.
 */
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

/**
 * <nodeUnaryMinus()>
 * Creates a node for unary minus factor.
 * Tree for unary minus is created as 0-(Factor)
 */
void nodeUnaryMinus() {
	NODE* data = createDataNode(0.0);
	NODE* new = popStack();

	pushStack(data);
	pushStack(new);
	new = stackReduce('-');
	pushStack(new);
}

/**
 * <node ~~ ()>
 * Constructs a syntax tree for each operator.
 * Then pushes the pointer to such tree in stack,
 * for remaining expression.
 */

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

/**
 * <initializeRoot()>
 * Newly create NODE variable,
 * then initialize its pointer to NULL
 */
void initializeRoot() {
	root = (NODE*)malloc(sizeof(NODE));
	root->left = NULL;
	root->right = NULL;
	stackIndex = 0;
}

/**
 * <create~~Node(double data)>
 * Creates a node for number literal, opreator, and variable.
 **/
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

/**
 * <pushStack(NODE* data)>
 * Push node to stack, temporarily
 */
void pushStack(NODE* data) {
	stack[stackIndex++] = data;
}

/**
 * <popStack()>
 * Pop a node from stack
 */
NODE* popStack() {
	return stack[--stackIndex];
}

/**
 * <stackReduce(char operator)>
 * - Input : An operator to reduce expression with.
 * Pops two nodes from stack, and append them to operator node.
 * Syntax Tree would be like below basically, possible for a tree to be merged.
 * i.e.)	+
 * 		   / \
 * 		  a   2
 */
NODE* stackReduce(char operator) {
	NODE* new = createOperatorNode(operator);
	new->right = popStack();
	new->left = popStack();

	root = new;

	return new;
}

/**
 * <traverseTree(NODE* node, int i, int j)>
 * - Input
 * 	- node : root of subtree to be traversed
 *	- i : Width depth of a tree
 * 	- j : Height depth of a tree 
 * Visually describes a syntax tree.
 * Traverses as in-order.
 * Entire tree traverse is driven by 'traverseTree(root, 0, 0)'
 * Left node is i=i+1 , j=j+1, and Right node is i=i+1
 * */
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

/**
 * <freeTree(NODE* node)>
 * - Input
 *	- node : root of subtree to be freed.
 * Frees the memory of dynamically allocated nodes.
 * Works as a preorder.
 * */
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
