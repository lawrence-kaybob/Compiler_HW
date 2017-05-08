#include <stdio.h>
#include <math.h>
#include "defs.h"
#include "evaluator.h"

/**
 * <Variables regarding value table>
 * Following variables are related to constructing value table
 * of variables in symbol table.
 * - varValue : Value table of variables. Indices follows that of symbol table.
 * - entryNo : External variable from equationParser.l (or equationParser.flex.cpp)
 *			   to keep track of trailer of symbol table
 */
double varValue[MAXENTRY];		
extern int entryNo;

/**
 * <evaluate(NODE* root)
 * - Argument
 *	- root : Root node of subtree to be evaluated.
 * Evaluate the tree from root.
 * Evaulation is cascaded from smaller to larger syntax tree
 */
double evaluate(NODE* root) {
	double result;

	switch (root->type) {			
	case DATA:						// If the node contains the number literal,
		return root->content.data;	// it returns its value
	case VARIABLE:									// If the node contains the index of table,
		return  evalVariable(root->content.index);	// it checks its index, and construct its node
	case OPERATOR: {														// If the node contains operator,
		switch (root->content.operator) {									// it checks which operator it has
		case '+':															// then, it returns the result of
			return evalPlus(evaluate(root->left), evaluate(root->right));	// that operator.
		case '-':
			return evalMinus(evaluate(root->left), evaluate(root->right));
		case '*':
			return evalMultiply(evaluate(root->left), evaluate(root->right));
		case '/':
			return evalDivide(evaluate(root->left), evaluate(root->right));
		case '=':
			return evalAssignment(root->left->content.index, evaluate(root->right));
		}
	}
	default:
		return NAN;
	}
}

/**
 * <printFormatted(double input)>
 * - Argument
 *	- input : Value to be printed
 * Checks the value contains floating point,
 * then prints as matched type, integer or floating point
 */
void printFormatted(double input) {
	int casted = (int)input;
	double check = input - casted;

	if (check > 0.0000001 || check < -0.0000001)
		printf("< %f\n", input);
	else
		printf("< %d\n", casted);
}

/**
 * eval~~(double num1, double num2)>
 * - Argument
 *	- num1, num2 : number to be evaluated with specific operator
 * Returns the result of operator
 */
double evalPlus(double num1, double num2)
{
	return num1 + num2;
}

double evalMinus(double num1, double num2)
{
	return num1 - num2;
}

double evalMultiply(double num1, double num2)
{
	return num1 * num2;
}

double evalDivide(double num1, double num2)
{
	if (num2 != 0.0)
		return num1 / num2;
	else
		return NAN;
}


/**
 * <evalVariable(int index)>
 * - Argument
 *	- index : index of variable in symbol table
 * Evaluate the variable node. Returns the value of it*/
double evalVariable(int index)
{
	if (index >= entryNo) {
		fprintf(stderr, "Variable not defined");
		return NAN;
	}
	else {
		return varValue[index];
	}
}

/**
 * <evalAssignment(int index, double val)>
 * - Argument
 *	- index : an index of an variable in symbol table
 *	- val : number literal to assign
 * Evaluate the index of variable in symbol table, then assign the number
 * to variable. If the variable is new, intialize it. */
double evalAssignment(int index, double val) {
	if (index >= entryNo) {
		varValue[entryNo++] = val;
	}
	else {
		varValue[index] = val;
	}

	return val;
}