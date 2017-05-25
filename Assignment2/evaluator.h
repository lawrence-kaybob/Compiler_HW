#ifndef EVALUATOR__
#define EVALUATOR__
#include "syntaxTree.h"

double evaluate(NODE* root);
void printFormatted(double input);

double evalPlus(double num1, double num2);
double evalMinus(double num1, double num2);
double evalMultiply(double num1, double num2);
double evalDivide(double num1, double num2);

double evalVariable(int index);
double evalAssignment(int index, double var);
#endif
