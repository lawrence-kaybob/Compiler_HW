%{
#include <string.h>
#include "equationParser.tab.h"

extern YYSTYPE yylval;
extern int symbolTableCheck();

char varName[100][100] = {0};
int entryNo = 0;
%}

digit [0-9]
integer [1-9]+{digit}*
floatPoint (([1-9]+|({digit}*\.{digit}+))([eE][-+]?{digit}+)?)

idPrefix	[a-zA-Z_]
identifier	{idPrefix}({idPrefix}|{digit})*

%%
{integer}|{floatPoint}|{digit}	{yylval.value = atof(yytext); return NUM;}
{identifier}					{yylval.index = symbolTableCheck(); return IDENTIFIER;}

\+	{return PLUS;}
\-	{return MINUS;}
\*	{return MULTIPLY;}
\/	{return DIVIDE;}
\(	{return LEFTPARAN;}
\)	{return  RIGHTPARAN;}
\=	{return ASSIGN;}

[ \t] ; /* ingore white space */
\n	{return END;}
.	{yyerror("Lexical Error");}

%%

int symbolTableCheck() {
	int i;
	char str[100] = {0};

	strncpy(str, yytext, yyleng);
	for(i = 0; i < entryNo; i++) {
		if(strcmp(str, varName[i]) == 0)
			return i;
	}

	sprintf(varName[i], "%s\0", str);
	return i;
}