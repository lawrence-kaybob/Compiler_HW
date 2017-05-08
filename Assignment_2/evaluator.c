#include <stdio.h>
#include <math.h>
#include "defs.h"
#include "evaluator.h"
	
// double eval[10];	// ����ϴ� ������ ����
// int evalIndex = 0;	// eval�� stackIndex

double varValue[MAXENTRY];
extern int entryNo;


double evaluate(NODE* root) {
	double result;

	switch (root->type) {
	case DATA:
		return root->content.data;
	case VARIABLE:
		return  evalVariable(root->content.index);
	case OPERATOR: {
		// Pop�ϸ鼭 �����ں��� ����� ��
		switch (root->content.operator) {
		case '+':
			return evalPlus(evaluate(root->left), evaluate(root->right));
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

void printFormatted(double input) {
	int casted = (int)input;
	double check = input - casted;

	if (check > 0.0000001 || check < -0.0000001)
		printf("< %f\n", input);
	else
		printf("< %d\n", casted);
}

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


// nodeVariable�� ���� ���鼭 �����ؾ��� ��
// �� ���� �ذ�� �� ������ ������....
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

double evalAssignment(int index, double val) {
	if (index >= entryNo) {
		varValue[entryNo++] = val;
	}
	else {
		varValue[index] = val;
	}

	return val;
}