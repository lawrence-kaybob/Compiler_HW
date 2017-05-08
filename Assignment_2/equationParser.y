%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include "equationParser.tab.h"

typedef union{
	double data;
	char operator;
	int index;
}_CONTENT;

typedef struct node{
	struct node *left;
	struct node *right;
	short type;
	_CONTENT content;	
}NODE;

NODE* root;
NODE* stack[20];
int stackIndex = 0;

void printFormatted(double input);

double evalExpr(double val);
double evalAssignment(int index, double val);
double evalTerm(double val);
double evalFactor(double val);
double evalNum(double val);
double evalVariable(int index);
double evalUnaryMinus(double val);
double evalParan(double val);

double evalPlus(double num1, double num2);
double evalMinus(double num1, double num2);
double evalMultiply(double num1, double num2);
double evalDivide(double num1, double num2);

NODE* initializeRoot();
NODE* createDataNode(double data);
NODE* createOperatorNode(char operator);
NODE* createStringNode(int index);
void pushStack(NODE* data);
NODE* popStack();
NODE* stackReduce(char operator);

void traverseTree(NODE* node, int i, int j);
void freeTree(NODE* node);

double varValue[100] = {0};
extern char varName[100][100];
extern int entryNo;

void yyerror(const char *message)
{
  fprintf(stderr, "error: %s\n", message);
}
 
int yywrap()
{
	return 0;
} 
  
main()
{
	root = initializeRoot();
	printf("> ");
	yyparse();
} 
%}
%union {
	double value;
	int index;
}
%token <value> NUM
%token <index> IDENTIFIER
%token PLUS MINUS MULTIPLY DIVIDE LEFTPARAN RIGHTPARAN ASSIGN END

%type <value> expr term factor line
%%

lines:
	line END					{printFormatted($1);}
	| lines line END			{printFormatted($2);}
	;

line:
	expr						{$$ = evalExpr($1);}
	| IDENTIFIER ASSIGN line	{$$ = evalAssignment($1, $3);}
	;

expr:
	expr PLUS term			{$$ = evalPlus($1, $3);}
	|	expr MINUS term		{$$ = evalMinus($1, $3);}
	|	term				{$$ = evalTerm($1);}
	;

term:
	term MULTIPLY factor		{$$ = evalMultiply($1, $3);}
	| term DIVIDE factor		{$$ = evalDivide($1, $3);}
	| factor					{$$ = evalFactor($1);}
	;

factor:
	LEFTPARAN line RIGHTPARAN	{$$ = evalParan($2);}
	| NUM						{$$ = evalNum($1);}
	| IDENTIFIER				{$$ = evalVariable($1);}
	| MINUS factor				{$$ = evalUnaryMinus($2);}
	;

%%

void printFormatted(double input){
	int casted = (int) input;
	double check = input - casted;

	if(isnan(input)) {
		if(root->left != NULL || root->right != NULL)
			freeTree(root);
		free(root);
		root = initializeRoot();
		printf("> ");
		return ;
	}
	
	traverseTree(root,0,0);
	freeTree(root);
	free(root);
	root = initializeRoot();

	if(check > 0.0000001 || check < -0.0000001)
		printf("< %f\n> ", input);
	else
		printf("< %d\n> ", casted);
}

double evalExpr(double val){
	return val;
}

double evalAssignment(int index, double val){
	NODE* new;
	NODE* variable;
	
	if(index >= entryNo) {
		variable = createStringNode(entryNo);
		varValue[entryNo++] = val;
	}
	else{
		variable = createStringNode(index);
		varValue[index] = val;
	}

	pushStack(variable);
	new = stackReduce('=');
	pushStack(new);

	return val;
}

double evalTerm(double val){
	return val;
}

double evalFactor(double val){
	return val;
}

double evalNum(double val){
	NODE* new = createDataNode(val);
	pushStack(new);

	return val;
}

double evalVariable(int index){
	if(index >= entryNo){
		yyerror("Variable not defined");
		return NAN;
	}
	else {
		NODE* new = createStringNode(index);
		pushStack(new);
		return varValue[index];	
	}
}

double evalUnaryMinus(double val){
	NODE* data = createDataNode(0.0);
	NODE* new = popStack();
	
	pushStack(data);
	pushStack(new);
	new = stackReduce('-');
	pushStack(new);
	return -val;
}

double evalParan(double val){
	return val;
}

double evalPlus(double num1, double num2){
	NODE* new = stackReduce('+');
	pushStack(new);
	return num1 + num2;
}

double evalMinus(double num1, double num2){
	NODE* new = stackReduce('-');
	pushStack(new);
	return num1 - num2;
}

double evalMultiply(double num1, double num2){
	NODE* new = stackReduce('*');
	pushStack(new);
	return num1 * num2;
}

double evalDivide(double num1, double num2) {
	if(num2 == 0) {
		yyerror("Divide by Zero");
	}
	else {
		NODE* new = stackReduce('/');
		return num1 / num2;
	}
}

NODE* initializeRoot(){
	root = (NODE*)malloc(sizeof(NODE));
	root->left = NULL;
	root->right = NULL;
	stackIndex = 0;
}

NODE* createDataNode(double data) {
	NODE* new = (NODE*) malloc(sizeof(NODE));
	new->left = NULL;
	new->right = NULL;
	new->content.data = data;
	new->type = 0;

	root = new;

	return new;
}

NODE* createOperatorNode(char operator) {
	NODE* new = (NODE*) malloc(sizeof(NODE));
	new->left = NULL;
	new->right = NULL;
	new->content.operator = operator;
	new->type = 1;

	return new;
}

NODE* createStringNode(int index){
	NODE* new = (NODE*) malloc(sizeof(NODE));
	new->left = NULL;
	new->right = NULL;
	new->content.index = index;
	new->type = 2;

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
	if(node->type == 0)
		printf("(%d,%d)\t%f\n", i, j, node->content.data);
	else if(node-> type == 1)
		printf("(%d,%d)\t%c\n", i, j, node->content.operator);
	else
		printf("(%d,%d)\t%s\n", i, j, varName[node->content.index]);

	if(node->left != NULL)
		traverseTree(node->left, i+1, j);
	
	if(node->right != NULL)
		traverseTree(node->right, i+1, j+1);
}

void freeTree(NODE* node) {
	if(node->left != NULL){
		freeTree(node->left);
		free(node->left);
	}
	if(node->right != NULL){
		freeTree(node->right);
		free(node->right);
	}
}