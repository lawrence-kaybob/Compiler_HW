%{
#include "defs.h"
%}

int int
char char
double double
void void
if if
else else
for for
while while
return return

whitespace	[ \t\n]
capital [A-Z]
lowercase [a-z]
digit [0-9]
underscore _
quotation \'
doubleQuotation \"
semicolon \;
colon \:

plus "+"
minus "-"
mult "*"
div "/"
modulo "%"
negation "!"
binaryAnd "&"
binaryOr "|"
questionMark "?"
leftParanthesis "("
rightParanthesis ")"
leftBrace "["
rightBrace "]"
leftBracket "{"
rightBracket "}"
comma	","
preprocessor "#"

defineProcessor {preprocessor}define
includeProcessor {preprocessor}include

less \<
greater \>
assign \=

idPrefix {capital}|{lowercase}|{underscore}
identifier {idPrefix}({idPrefix}|{digit})*

standardHeader "<"([^(.h)])*.h">"

integer (\+|\-)?[1-9]+{digit}*
floatPoint -?(([1-9]+|({digit}*\.{digit}+))([eE][-+]?{digit}+)?)

string {doubleQuotation}([^"']|(\\['"]))*{doubleQuotation}
singleCharacter {quotation}([^'"]|(\\['"nt])){quotation}

comment "/*"([^*]|(\*+[^*/]))*\*+\/

plusOne {plus}{plus}
minusOne {minus}{minus}

lessEqual {less}{equals}
greaterEqual {greater}{equals}
equals {assign}{assign}
notEqual {negation}{assign}

assignPlus {plus}{assign}
assignMinus {minus}{assign}
assignMult {mult}{assign}
assignDiv {div}{assign}
assignModulo {modulo}{assign}

logicAnd {binaryAnd}{binaryAnd}
logicOr {binaryOr}{binaryOr}

%%

{int}		{return INT;}
{char}		{return CHAR;}
{double}	{return DOUBLE;}
{void}		{return VOID;}
{if}		{return IF;}
{else}		{return ELSE;}
{for}		{return FOR;}
{while}		{return WHILE;}
{return}	{return RETURN;}

{digit}			{return DIGIT;}
{identifier}	{return IDENTIFIER;}
{integer}		{return INTEGER;}
{floatPoint}	{return FLOATPOINT;}

{string}		{return STRING;}
{comment}		{return COMMENT;}
{whitespace}	{return WHITESPACE;}
{quotation}		{return QUOTATION;}
{doubleQuotation} {return DOUBLEQUOTATION;}
{singleCharacter}	{return SINGLECHARACTER;}

{semicolon}		{return SEMICOLON;}
{colon}			{return COLON;}

{plus}			{return PLUS;}
{minus}			{return MINUS;}
{mult}			{return MULTIPLY;}
{div}			{return DIVIDE;}
{modulo}		{return MODULO;}
{negation}		{return NEGATION;}

{binaryAnd} {return BINARYAND;}
{binaryOr}	{return BINARYOR;}

{plusOne}	{return PLUSONE;}
{minusOne}	{return MINUSONE;}

{less}		{return LESS;}
{greater}	{return GREATER;}
{assign}	{return ASSIGN;}

{lessEqual}	{return LESSEQUAL;}
{greaterEqual} {return GREATEREQUAL;}
{equals}		{return EQUAL;}
{notEqual}		{return NOTEQUAL;}

{assignPlus}		{return ASSIGNPLUS;}
{assignMinus}	{return ASSIGNMINUS;}
{assignMult}	{return ASSIGNMULT;}
{assignDiv}		{return ASSIGNDIV;}
{assignModulo}	{return ASSIGNMODULO;}

{logicAnd}		{return LOGICAND;}
{logicOr}		{return LOGICOR;}

{leftParanthesis}	{return LEFTPARANTHESIS;}
{rightParanthesis}	{return RIGHTPARANTHESIS;}
{leftBracket}		{return LEFTBRACKET;}
{rightBracket}		{return RIGHTBRACKET;}
{leftBrace}			{return LEFTBRACE;}
{rightBrace}		{return RIGHTBRACE;}

{comma}		{return COMMA;}
{includeProcessor} {return INCLUDEPROCESSOR;}
{standardHeader}	{return STANDARDHEADER;}
{defineProcessor}	{return DEFINEPROCESSOR;}
%%

#include <stdio.h>
#include <string.h>

#define TABLEMAX 100
extern int yylex();
extern char *yytext;
extern int yyleng;

int main(int argc, char **argv) {
	int tok;
	int i;
	int dup = 0;
	
	int idIndex = 0;
	int strIndex = 0;
	char* identifierTable[TABLEMAX];
	char* stringTable[TABLEMAX];

	if(argc == 2) {
		FILE *file;
		file = fopen(argv[1], "r");
		if(!file) {
			fprintf(stderr, "Could not open %s\n", argv[1]);
			exit(1);
		}
		yyin = file;
	}
	else {
		fprintf(stderr, "Only one file per execution allowed\n");
		exit(2);
	}

	printf("TOKEN\t\tLEXEME\n");
	printf("-----\t\t------\n");

	while((tok = yylex()) != 0) {
		switch(tok) {
			case INT:
				printf("<INT>");
				break;
			case CHAR:
				printf("<CHAR>");
				break;
			case DOUBLE:
				printf("<DOUBLE>");
				break;
			case VOID:
				printf("<VOID>");
				break;
			case IF:
				printf("<IF>");
				break;
			case ELSE:
				printf("<ELSE>");
				break;
			case FOR:
				printf("<FOR>");
				break;
			case WHILE:
				printf("<WHILE>");
				break;
			case RETURN:
				printf("<RETURN>");
				break;
			case IDENTIFIER:
				printf("<IDENTIFIER, ");
				identifierTable[idIndex] = (char*) calloc(sizeof(char), yyleng);
				strncpy(identifierTable[idIndex++], yytext, (yyleng > 16 ? 16 : yyleng));
				for(i = 0; i < idIndex - 1; i++) {
					if(strcmp(identifierTable[i], identifierTable[idIndex-1]) == 0) {
						printf("%d>", i+1);
						dup = 1;
						idIndex--;
						break;
					} 
				}

				if(dup){
					dup = 0;
					break;
				}
				else{
					printf("%d>", i+1);
					break;
				}
			case INTEGER:
				printf("<INTEGER>");
				printf("\t\t%.10s\n", yytext);
				continue;
			case FLOATPOINT:
				printf("<FLOATPOINT>");
				break;
			case STRING:
				printf("<STRING, ");
				stringTable[strIndex] = (char*) calloc(sizeof(char), yyleng);
				strncpy(stringTable[strIndex++], yytext, yyleng);
				for(i = 0; i < strIndex - 1; i++) {
					if(strcmp(stringTable[i], stringTable[strIndex-1]) == 0) {
						printf("%d>", i+1);
						dup = 1;
						strIndex--;
						break;
					} 
				}
				if(dup){
					dup = 0;
					break;
				}
				else{
					printf("%d>", i+1);
					break;
				}
			case COMMENT:
				printf("<COMMENT>");
				break;
			case PLUS:
				printf("<PLUS>");
				break;
			case MINUS:
				printf("<MINUS>");
				break;
			case MULTIPLY:
				printf("<MULTIPLY>");
				break;
			case DIVIDE:	
				 printf("<DIVIDE>");
				 break;
			 case MODULO:
			 	printf("<MODULO>");
			 	break;
		 	case NEGATION:
		 		printf("<NEGATION>");
	 			break;
 			case BINARYAND:
 				printf("<BINARYAND>");
 				break;
			case BINARYOR:
				printf("<BINARYOR>");
				break;
			case PLUSONE:
				printf("<PLUSONE>");
				break;
			case MINUSONE:
				printf("<MINUSONE>");
				break;
			case LESS:
				printf("<LESS>");
				break;
			case GREATER:
				printf("<GREATER>");
				break;
			case ASSIGN:
				printf("<ASSIGN>");
				break;
			case LESSEQUAL:
				printf("<LESSEQUAL>");
				break;
			case GREATEREQUAL:
				printf("<GREATEREQUAL>");
				break;
			case EQUAL:
				printf("<EQUAL>");
				break;
			case NOTEQUAL:
				printf("<NOTEQUAL>");
				break;
			case ASSIGNPLUS:
				printf("<ASSIGNPLUS>");
				break;
			case ASSIGNMINUS:
				printf("<ASSIGNMINUS>");
				break;
			case ASSIGNMULT:
				printf("<ASSIGNMULT>");
				break;
			case ASSIGNMODULO:
				printf("<ASSIGNMODULO>");
				break;
			case LOGICAND:
				printf("<LOGICAND>");
				break;
			case LOGICOR:
				printf("<LOGICOR>");
				break;
			case SEMICOLON:
				printf("<SEMICOLON>");
				break;
			case WHITESPACE:
				continue;
			case DOUBLEQUOTATION:
				printf("<DOUBLEQUOTATION>");
				break;
			case LEFTPARANTHESIS:
				printf("<LEFTPARANTEHSIS>");
				break;
			case RIGHTPARANTHESIS:
				printf("<RIGHTPARANTEHSIS>");
				break;
			case LEFTBRACE:
				printf("<LEFTBRACE>");
				break;
			case RIGHTBRACE:
				printf("<RIGHTBRACE>");
				break;
			case LEFTBRACKET:
				printf("<LEFTBRACKET>");
				break;
			case RIGHTBRACKET:
				printf("<RIGHTBRACKET>");
				break;
			case COMMA:
				printf("<COMMA>");
				break;
			case INCLUDEPROCESSOR:
				printf("<INCLUDE>");
				break;
			case STANDARDHEADER:
				printf("<STANDARD HEADER>");
				break;
			case DEFINEPROCESSOR:
				printf("<DEFINE>");
				break;
			case SINGLECHARACTER:
				printf("<SIGNLECHARACTER>");
				break;
			case COLON:
				printf("<COLON>");
				break;
			case DIGIT:
				printf("<DIGIT>");
				break;
			case QUOTATION:
				printf("<SINGLEQUOTATION>");
				break;
			default:
				printf("[Error] Cannot recognize the pattern of : %s\n", yytext);
				break;

		}
		printf("\t\t%s\n", yytext);
	}

	printf("\nINDEX\t\tSymbols\n");
	printf("-----\t\t-------\n");

	for(i = 0; i < idIndex; i++) {
		printf("%d\t\t%s\n", i+1, identifierTable[i]);
		free(identifierTable[i]);
	}

	printf("\nINDEX\t\tStrings\n");
	printf("-----\t\t-------\n");

	for(i = 0; i < strIndex; i ++) {
		printf("%d\t\t%s\n", i+1, stringTable[i]);
		free(stringTable[i]);
	}


}