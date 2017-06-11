%{
#include <string.h>
#include "equationParser.Assignment3.tab.h"
#include "defs.h"

extern YYSTYPE yylval;
extern int symbolTableCheck();
extern void printDangling();

char varName[100][100] = {0};
int entryNo = 0;
int inControl = 0;
extern int localEntryNo;
%}

if if
else else
while while
end end

digit [0-9]
integer [1-9]+{digit}*
floatPoint (([1-9]+|({digit}*\.{digit}+))([eE][-+]?{digit}+)?)

idPrefix	[a-zA-Z_]
identifier	{idPrefix}({idPrefix}|{digit})*

%%
{if}	{return IF;}
{else} {return ELSE;}
{while}	{return WHILE;}
{end} {return END;}

{integer}|{floatPoint}|{digit}	{yylval.value = atof(yytext); return NUM;}
{identifier}					{yylval.index = symbolTableCheck(); return IDENTIFIER;}

\=\=	{return EQUAL;}
\=\!	{return NOTEQUAL;}

\+	{return PLUS;}
\-	{return MINUS;}
\*	{return MULTIPLY;}
\/	{return DIVIDE;}
\(	{return LEFTPARAN;}
\)	{return  RIGHTPARAN;}
\=	{return ASSIGN;}

\<	{return LESS;}
\>	{return GREATER;}

[ \t] ; /* ingore white space */
\n	{printDangling();}
\;	{return EOL;}
.	{printf("Lexical Error\n");}

%%

int symbolTableCheck() {
	int i;
	char str[100] = {0};

	strncpy(str, yytext, yyleng);
	for(i = 0; i < entryNo; i++) {			// Iterates through symbol table
		if(strcmp(str, varName[i]) == 0)	// If there is a match,
			return i;						// it returns its index.
	}

	sprintf(varName[localEntryNo], "%s\0", str);	// If there is no such identifier in symbol table
	return localEntryNo++;							// Add it temporarily.
}

void printDangling() {
	if(inControl) {
		printf("> ");
	}
}