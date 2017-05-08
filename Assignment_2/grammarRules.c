#include "defs.h"
#include "syntaxTree.h"
#include "evaluator.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

extern int yylex();
extern int yyleng;
extern char* yytext;

TOKEN token;
extern NODE* root;

int exprStart(int tok);
int expr(int tok);
int termStart(int tok);
int term1(int tok);
int term2(int tok);
int factorAll(int tok);
int factor1(int tok);
int factor2(int tok);
int getNextToken();

void parse() {
	int tok;
	
	printf("> ");

	tok = getNextToken();
	tok = exprStart(tok);

	switch (tok) {
		case END: {
			double result = evaluate(root);
			printFormatted(result);

			freeTree(root);
			free(root);
			initializeRoot();
			break;
		}
		default: {
			int EOLCheck = strcmp(yytext, "\n");
			printf("Error : Syntax Error\n");
			while (tok != END && EOLCheck != 0)
				tok = getNextToken();
			break;
		}
	}

}

int exprStart(int tok) {
	int localTok = termStart(tok);
	localTok = expr(localTok);

	return localTok;
}

int expr(int tok) {

	if (tok != ERROR) {
		int localTok;
		switch (tok) {
			case PLUS: {
				localTok = termStart(getNextToken());
				if (localTok != ERROR) {
					nodePlus();
					localTok = expr(localTok);
				}
				break;
			}
			case MINUS: {
				localTok = termStart(getNextToken());
				if (localTok != ERROR) {
					nodeMinus();
					localTok = expr(localTok);
				}
				break;
			}
			default:
				localTok = tok;
		}
		return localTok;

	}
	return ERROR;
}

int termStart(int tok) {
	int localTok;

	if (tok != ERROR) {
		switch (tok) {
		case LEFTPARAN:
		case NUM:
		case MINUS:
			localTok = factor1(tok);
			localTok = term1(localTok);
			break;
		case IDENTIFIER:
			localTok = factor2(tok);
			localTok = term2(localTok);
			break;
		}
		return localTok;
	}
}

int term1(int tok)
{
	if (tok != ERROR) {
		int localTok;
		switch (tok) {
		case MULTIPLY: {
			localTok = factorAll(getNextToken());
			if (localTok != ERROR) {
				nodeMultiply();
				localTok = term1(localTok);
			}
			break;
		}
		case DIVIDE: {
			localTok = factorAll(getNextToken());
			if (localTok != ERROR) {
				nodeDivide();
				localTok = term1(localTok);
			}
			break;
		}
		default:
			localTok = tok;
		}
		return localTok;
	}
}

int term2(int tok)
{
	if (tok != ERROR) {
		int localTok;
		switch (tok) {
			case ASSIGN: {
				int index;
				localTok = getNextToken();
				index = token.index;
				localTok = exprStart(localTok);
				if (localTok != ERROR)
					nodeAssignment();
				break;
			}
			default:
				localTok = term1(tok);
		}
		return localTok;
	}
}

int factorAll(int tok)
{
	if (tok != ERROR) {
		int localTok;
		
		switch (tok) {
		case LEFTPARAN:
		case NUM:
		case MINUS:
			localTok = factor1(tok);
			break;
		case IDENTIFIER:
			localTok = factor2(tok);
			break;
		}
		return localTok;
	}
}

int factor1(int tok)
{
	int localTok;

	if (tok != ERROR) {
		switch (tok) {
			case LEFTPARAN:
				localTok = exprStart(getNextToken());
				if (localTok == RIGHTPARAN)
					localTok = getNextToken();
				else
					localTok = ERROR;
				break;
			case NUM: {
				nodeNum(token.value);
				localTok = getNextToken();
				break;
			}
			case MINUS: {
				localTok = factorAll(getNextToken());
				nodeUnaryMinus(token.value);
				break;
			}
			default: {
				localTok = tok;
			}
		}
		return localTok;
	}

}



int factor2(int tok)
{
	int localTok;

	if (tok != ERROR) {
		switch (tok) {
			case IDENTIFIER: {
				localTok = getNextToken();
				if (nodeVariable(token.index, localTok) == ERROR)
					localTok = ERROR;
				break;
			}
			default:
				localTok = tok;
		}
		return localTok;
	}
}

int getNextToken() {
	return yylex();
}