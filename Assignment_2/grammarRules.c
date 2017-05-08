/**
 * grammarRules.c : Defines syntax of expressions
 * - Non-terminals
 *	- E : Expression, with rest expression (E')
 *	- T : Term. with term 1(T1) & term2(T2)
 *	- F : Factor, with factor 1(F1) & factor 2(F2)
 * - Termianals
 *	- num : Number literal, regardless of floating point
 *	- id : Variable, following a rule of identifier
 *	- ( ) : Paranthesis
 *	- +, -, *, / : Operators
 *	- end epsillon: End of line, which is \n
 * - Grammar Rules
 * 	- E  -> TE'
 *	- E' -> +TE' | -TE' | epsillon
 *	- T  -> F1T1 | F2T2
 *	- T1 -> *F1T1 | /F1T1 | epsillon
 *	- T2 -> =E | T1
 *	- F  -> F1 | F2
 *	- F1 -> (E) | num | -F
 *	- F2 -> id
 */
#include "defs.h"
#include "syntaxTree.h"
#include "evaluator.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * <Decalrations related to flex>
 * - yylex() : Traverses the input & returns next token
 * - yytext : Pointer to string of current token
 */
extern int yylex();
extern char* yytext;

/**
 * <Varaibles regarding parsing>
 * - token : Variable for getting actual value that token stands for
 *			 i.e.) Variable index in symbol table, or value of number literal
 * - root : External variable from syntaxTree.c
 *			Root node of entire syntax tree of equation
 */
TOKEN token;
extern NODE* root;

// Following are declaration of fucntions, used in grammarRules.c
int exprStart(int tok);
int expr(int tok);
int termStart(int tok);
int term1(int tok);
int term2(int tok);
int factorAll(int tok);
int factor1(int tok);
int factor2(int tok);
int getNextToken();

/**
 * <parse()>
 * Driver function to start parsing.
 * Start by checking rule #1
 */
void parse() {
	int tok;
	
	printf("> ");

	tok = getNextToken();
	tok = exprStart(tok);						// If expression seems to be finished

	switch (tok) {
		case END: {								// Checks if it reached end of line
			double result = evaluate(root);		// If it does, it means it is valid expression
			printFormatted(result);				// So, it iterates through syntax tree, and evaluate
												// the result value.
			freeTree(root);						// After evaluation is done, we free the memory
			free(root);							// of nodes, clearing syntax tree for
			initializeRoot();					// next expression
			break;
		}
		default: {
			int EOLCheck = strcmp(yytext, "\n");// If it has not reached end of line,
			printf("Error : Syntax Error\n");	// or encountered error during parsing,
			while (tok != END && EOLCheck != 0)	// it clears the input, getting ready
				tok = getNextToken();			// to get next expression.
			break;
		}
	}

}
/**
 * <exprStart(int tok)
 * -Input
 *	- tok : Token to be evaluated
 * Function defining rule #1.
 * Check the non-terminal T,then check remaining expression.
 */
int exprStart(int tok) {
	int localTok = termStart(tok);
	localTok = expr(localTok);

	return localTok;
}

/**
 * <expr(int tok)>
 * - Input
 *	- tok : Token to be evaluated
 * Function defining rule #2.
 */
int expr(int tok) {

	if (tok != ERROR) {
		int localTok;
		switch (tok) {
			case PLUS: {								// If + operator matches,
				localTok = termStart(getNextToken());	// it shifts toward next token.
				if (localTok != ERROR) {				// If there is no problem with the term,
					nodePlus();							// it binds two term with + operator,
					localTok = expr(localTok);			// then checks for another E
				}
				break;
			}
			case MINUS: {								// If - operator matches,
				localTok = termStart(getNextToken());	// it shifts toward next token.
				if (localTok != ERROR) {				// If there is no problem with the term,
					nodeMinus();						// it binds two term with - operator,
					localTok = expr(localTok);			// then checks for another E
				}
				break;
			}
			default:									// If there is no match for either operator,
				localTok = tok;							// it means, this is the end of E
		}
		return localTok;

	}
	return ERROR;
}
/**
 * <termStart(int tok)>
 * Function defining rule #3.
 */
int termStart(int tok) {
	int localTok;

	if (tok != ERROR) {
		switch (tok) {
		case LEFTPARAN:						// If there is a match with factor 1,
		case NUM:							// which is Left paranthesis, numberl iteral,
		case MINUS:							// and minus,
			localTok = factor1(tok);		// it checks for factor 1.
			localTok = term1(localTok);		// Then, it checks remaining term 1.
			break;
		case IDENTIFIER:					// If there is a match with factor 2, which is variable,
			localTok = factor2(tok);		// it checks for factor 2.
			localTok = term2(localTok);		// Then, it checks remaining term 2.
			break;
		}
		return localTok;					// End of rule #3.
	}
}
/**
 * <term1(int tok)>
 * - Input
 *	- tok : Token to be evaluated.
 * Function defining rule #4.
 */
int term1(int tok)
{
	if (tok != ERROR) {
		int localTok;
		switch (tok) {
		case MULTIPLY: {							// If there is a match with * operator,it shfits &
			localTok = factorAll(getNextToken());	// it checks for factor (either factor 1, or factor 2)
			if (localTok != ERROR) {				// If there is no problem with a factor,
				nodeMultiply();						// it binds two factor with * operator.
				localTok = term1(localTok);			// Then, it checks reamining term 1.
			}
			break;
		}
		case DIVIDE: {								// If there is a match with / operator, it shifts &
			localTok = factorAll(getNextToken());	// it also checks for factor (either factor 1, or factor 2)
			if (localTok != ERROR) {				// If there is no problem with a factor,
				nodeDivide();						// it binds two factor with / operator.
				localTok = term1(localTok);			// Then, it checks reaminig term 2.
			}
			break;
		}
		default:
			localTok = tok;
		}
		return localTok;
	}
}
/**
 * <term2(int tok)>
 * - Input
 *	- tok : Token to be evaluated
 * Function defining rule #5.
 */
int term2(int tok)
{
	if (tok != ERROR) {
		int localTok;
		switch (tok) {
			case ASSIGN: {						// If there is a match with = operator,
				int index;						// it shifts through next token, and evaluate next
				localTok = exprStart(getNextToken());
				if (localTok != ERROR)			// expression. If there is no problem with expression,
					nodeAssignment();			// it binds two factor with = operator
				break;
			}
			default:							// If there is no match, it sees the token as a variable
				localTok = term1(tok);			// to be accessed (not initializing), so it continues with
		}										// checking rule #4.
		return localTok;
	}
}

/**
 * <factorAll(int tok)>
 * - Input
 *	- tok : Token to be evaluated
 * Function defining rule #6.
 */
int factorAll(int tok)
{
	if (tok != ERROR) {
		int localTok;
		
		switch (tok) {					// If there is a match with factor 1,
		case LEFTPARAN:					// which is left paranthesis, number literal,
		case NUM:						// or minus operator (for unary minus),
		case MINUS:						// the token is evaluated as factor 1.
			localTok = factor1(tok);	// Then, it shifts toward next token
			break;
		case IDENTIFIER:				// If there is a match with factor 2, which is identifier,
			localTok = factor2(tok);	// the token is evaluated as factor 2, identifier
			break;						// then, it shifts toward next token.
		}
		return localTok;
	}
}

/**
 * <factor1(int tok)>
 * - Input
 *	- tok : Token to be evaluated
 * Function defining rule #7.
 */
int factor1(int tok)
{
	int localTok;

	if (tok != ERROR) {
		switch (tok) {									// If there is a match with left parantehsis,
			case LEFTPARAN:								// if shifts toward next token,
				localTok = exprStart(getNextToken());	// evaluating it as expression. After expression
				if (localTok == RIGHTPARAN)				// is evaluated, it checks a match with right
					localTok = getNextToken();			// paranthesis. If it does, it is accepted as factor 1,
				else									// If it's not, it is syntax error
					localTok = ERROR;
				break;
			case NUM: {									// If there is a match with number literal,
				nodeNum(token.value);					// it creates a node of number literal, with its value
				localTok = getNextToken();				// and shifts toward next token, returning to
				break;									// the caller function.
			}
			case MINUS: {								// If there is a match with minus operator,
				localTok = factorAll(getNextToken());	// it shifts toward next token, evaluating it as another
				if(localTok != ERROR)					// factor. If it does, it is accepted as factor 1,
					nodeUnaryMinus(token.value);		// creating a node for unary minus
				break;
			}
			default: {									// End of rule #7
				localTok = tok;
			}
		}
		return localTok;
	}

}


/**
 * <factor2(int tok)>
 * - Input
 *	- tok : Token to be evaluated
 * Function defining rule #8
 */
int factor2(int tok)
{
	int localTok;

	if (tok != ERROR) {
		switch (tok) {
			case IDENTIFIER: {										// If there is a match with identifier-ruled- string,
				localTok = getNextToken();							// it shift toward next token, as lookahead,
				if (nodeVariable(token.index, localTok) == ERROR)	// try to create a node for a variable
					localTok = ERROR;								// If it fails, stop the parsing
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