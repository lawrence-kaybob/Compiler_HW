%{
#include <stdio.h>
#include <math.h>
#include "syntaxTree.h"
#include "evaluator.h"
#include "equationParser.Assignment3.tab.h"

int yylex();

void yyerror(const char *message)
{
  fprintf(stderr, "error: %s\n", message);
}
 
int yywrap()
{
	return 0;
} 
%}

%union {
	double value;
	int index;
}
%token <value> NUM
%token <index> IDENTIFIER
%token PLUS MINUS MULTIPLY DIVIDE GREATER LESS EQUAL NOTEQUAL LEFTPARAN RIGHTPARAN ASSIGN EOL IF ELSE WHILE END


%type <value> expr term factor line statement isEqual

%right ASSIGNEQUAL

%%

statements:
	statement				
	| statements statement
	;

statement:
	if statements else statements end		{}	
	| while statements end					{}
	| line EOL								{treeLine();}
	;

if:
	IF LEFTPARAN line RIGHTPARAN	{treeIf();}
	;

else:
	ELSE	{treeElse();}
	;

while:
	WHILE LEFTPARAN line RIGHTPARAN	{treeWhile();}
	;

end:
	END	{traverseBlock();}
	;

line:
	IDENTIFIER ASSIGN line		{nodeAssignment($1);}
	| expr GREATER isEqual		{nodeGreater($3);}
	| expr LESS isEqual			{nodeLess($3);}
	| expr EQUAL expr			{nodeEqual();}
	| expr NOTEQUAL expr	{nodeNotEqual();}
	| expr
	;

isEqual:
	ASSIGN expr	{$$ = 1;}
	| expr		{$$ = 0;}
	;

expr:
	expr PLUS term			{nodePlus();}
	|	expr MINUS term		{nodeMinus();}
	|	term				{}
	;

term:
	term MULTIPLY factor		{nodeMultiply();}
	| term DIVIDE factor		{nodeDivide();}
	| factor					{}
	;

factor:
	LEFTPARAN line RIGHTPARAN	{}
	| NUM						{nodeNum($1);}
	| IDENTIFIER				{nodeVariable($1);}
	| MINUS factor				{nodeUnaryMinus();}
	;

%%

void main()
{
	initializeRoot();
	printf("?-  ");
	yyparse();
}

/*
 * <Syntax Tree Related Variables>
 * Following variables are essential for constructing
 * syntax tree of an expression
 */
NODE* root;				// Root node of syntax tree
NODE* stack[20];		// Temporary stack for nodes to be constructed
int stackIndex = 0;		// Trailer variable of stack

TREE* head = NULL;
TREE* tail = NULL;

/*
 * <Symbol Table Related Variable>
 * Following variables are essential for recognizing
 * variable name & its value
 */

// External Variable imported from equationParser.l or equationParser.flex.cpp
extern char varName[100][100];	// Symbol Table for variable name
extern int entryNo;				// Trailer variable for symbol table
extern int inControl;

double varValue[100] = { 0 };	// Value Table for variables in symbol Table
int localEntryNo = 0;			// Temporary entryNo variable for assigning new variables

// Pre-definition for some fucntions
void traverseTree(NODE* node, int i, int j);
void freeTree(NODE* node);

/**
 * <nodeAssignemnt()>
 * Binds two nodes with '='(Assign) operator.
 * Had variable to be left of the operator
 * i.e.) a=2 -->	'='
 *				    / \
 *				   a   2
 */
void nodeAssignment(int index) {
	NODE* new;
	NODE* variable;

	variable = createStringNode(index);

	new = popStack();
	pushStack(variable);
	pushStack(new);
	new = stackReduce('=');
	pushStack(new);

	entryNo = localEntryNo;
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
void nodeVariable(int index) {

	if (index >= entryNo) {
		yyerror("Variable not defined");
		return NAN;
	}
	else {
		NODE* new = createStringNode(index);
		pushStack(new);
		return varValue[index];
	}
	
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

void nodeGreater(int equal) {
	NODE* new;
	if(equal)
		new = stackReduce('G');
	else
		new = stackReduce('>');
	pushStack(new);
}

void nodeLess(int equal) {
	NODE *new;
	if(equal)
		new = stackReduce('L');
	else
		new = stackReduce('<');
	pushStack(new);
}

void nodeEqual() {
	NODE* new = stackReduce('E');

	pushStack(new);
}

void nodeNotEqual() {
	NODE* new = stackReduce('N');

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

NODE* createOperatorNode(char operation) {
	NODE* new = (NODE*)malloc(sizeof(NODE));
	new->left = NULL;
	new->right = NULL;
	new->content.operation = operation;
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

NODE* createControlNode(char type) {
	NODE* new = (NODE*)malloc(sizeof(NODE));
	new->left = NULL;
	new->right = NULL;
	new->content.operation = type;
	new->type = CONTROL;

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
NODE* stackReduce(char operation) {
	NODE* new = createOperatorNode(operation);
	new->right = popStack();
	new->left = popStack();

	root = new;

	return new;
}

void traverseRoot() {
	double result;

	result = evaluate(root);
	if(!inControl)
		printFormatted(result);

	// traverseTree(root, 0, 0);
	freeTree(root);
	free(root);
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
	if (node->left != NULL)
		traverseTree(node->left, i + 1, j);

	if (node->type == DATA)
		printf("(%d,%d)\t%f\n", i, j, node->content.data);
	else if (node->type == OPERATOR)
		printf("(%d,%d)\t%c\n", i, j, node->content.operation);
	else
		printf("(%d,%d)\t%s\n", i, j, varName[node->content.index]);

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

void treeLine() {
	NODE* line = root;

	insertTree(line);

	if(!inControl) {
		traverseRoot();
		free(head);
		head = NULL;
		tail = NULL;
	}

}

void treeIf() {
	NODE* new = createControlNode('I');
	NODE* condition = popStack();

	inControl = 1;

	new->left = condition;	// 항상 왼쪽을 참고하도록!!
	insertTree(new);
}

void treeElse() {
	NODE* new = createControlNode('E');
	
	insertTree(new);
}

void treeWhile() {
	NODE* new = createControlNode('W');
	NODE* condition = popStack();

	inControl = 1;

	new->left = condition;
	insertTree(new);
}

void traverseBlock() {
	TREE* now = head;
	double value;
	int condition = 0;		// 조건문에서 조건의 참 거짓을 기억하는 변수
	

	while(now != NULL) {
		value = evaluate(now->statement);
		
		if(now->statement->type == CONTROL) {
			switch(now->statement->content.operation) {
				case 'I':
					printf("if\n");
					if(value >= 1.0){
						// 조건에 맞으니 우선 풀자
						condition = 1;
						now = now -> next;
					}
					else {
						while(now->statement->content.operation != 'E')
							now = now -> next;
					}
					break;
				case 'E':
					if(condition == 1) {
						now = NULL;
						break;
					}
					// else에 당도해서 condition == 0이면 그대로 전개하면 되는 것!
					// 아니면 빠져나오는 것이고
					else
						now = now -> next;
					break;
				case 'W':
					printf("while\n");
					// 참이면 계속 돌아가게하면 되지
					// 그게 바로 head로 돌아오게 하는 것!
					if(value >= 1.0) {
						tail -> next = head;
						now = now -> next;
					}
					else {
						tail -> next = NULL;
						now = NULL;
					}
					break;
			}
		}
		else {
			now = now -> next;
		}
	}

	freeBlock(head);
	head = NULL;
	tail = NULL;

	inControl = 0;
	printf("?- ");
}

void insertTree(NODE* new) {
	TREE* insert = (TREE*)malloc(sizeof(TREE));
	
	insert -> next = NULL;

	if(new->type == CONTROL && new->content.operation == 'E')
		head -> falseNext = insert;

	insert -> falseNext = NULL;
	insert -> statement = new;
	
	if(head == NULL)
		head = insert;
		
	if(tail != NULL)
		tail -> next = insert;
	tail = insert;
}

void freeBlock(TREE* tree) {
	TREE* now = tree;
	TREE* next = now;

	while(next != NULL) {
		freeTree(now->statement);
		free(now->statement);
		next = now -> next;
		if(next != NULL) {
			free(now);
			now = next;
		}
	}
}