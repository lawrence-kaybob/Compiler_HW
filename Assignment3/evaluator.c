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
	double synthesized;

	if (root->left != NULL) {
		synthesized = evaluate(root->left);
	}

	switch (root->type) {
		case DATA:
			synthesized = root->content.data;
			break;
		case VARIABLE:
			synthesized = evalVariable(root->content.index);
			break;
		case OPERATOR: {
			switch (root->content.operation) {
				case '+':
					synthesized = evalPlus(synthesized, evaluate(root->right));
					break;
				case '-':
					synthesized = evalMinus(synthesized, evaluate(root->right));
					break;
				case '*':
					synthesized = evalMultiply(synthesized, evaluate(root->right));
					break;
				case '/':
					synthesized = evalDivide(synthesized, evaluate(root->right));
					break;
				case '=':
					synthesized = evalAssignment(root->left->content.index, evaluate(root->right));
					break;
				case '>':
					synthesized = evalGreater(synthesized, evaluate(root->right));
					break;
				case '<':
					synthesized = evalLess(synthesized, evaluate(root->right));
					break;
				case 'G':
					synthesized = evalGreaterEqual(synthesized, evaluate(root->right));
					break;
				case 'L':
					synthesized = evalLessEqual(synthesized, evaluate(root->right));
					break;
				case 'E':
					synthesized = evalEqual(synthesized, evaluate(root->right));
					break;
				case 'N':
					synthesized = evalNotEqual(synthesized, evaluate(root->right));
					break;
			}
			break;
		}
		case CONTROL: {
			switch (root->content.operation) {
				case 'I':
				case 'W':
					synthesized = evaluate(root->left);
					break;
				case 'E':
					synthesized = NAN; // Dummy Value
			}
			break;
		}
		default:
			synthesized =  NAN;
	}

	return synthesized;
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

	if (isnan(input)) {
		printf("?- ");
		return;
	}

	if (check > 0.0000001 || check < -0.0000001)
		printf("! %f\n", input);
	else
		printf("! %d\n", casted);

	printf("?- ");
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
	else {
		yyerror("Variable Not Defined");
		return NAN;
	}
}

double evalGreater(double num1, double num2) {
	if (num1 > num2)
		return 1.0;
	else
		return 0.0;
}

double evalLess(double num1, double num2) {
	if (num1 < num2)
		return 1.0;
	else
		return 0.0;
}

double evalGreaterEqual(double num1, double num2) {
	if (num1 >= num2)
		return 1.0;
	else
		return 0.0;
}

double evalLessEqual(double num1, double num2) {
	if (num1 <= num2)
		return 1.0;
	else
		return 0.0;
}
double evalEqual(double num1, double num2) {
	if (num1 == num2)
		return 1.0;
	else
		return 0.0;
}
double evalNotEqual(double num1, double num2) {
	if (num1 != num2)
		return 1.0;
	else
		return 0.0;
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
 * to variable. If the variable is new, intialize it.
 */
double evalAssignment(int index, double val) {

	if (index >= entryNo) {
		varValue[entryNo++] = val;
	}
	else {
		varValue[index] = val;
	}

	return val;
}