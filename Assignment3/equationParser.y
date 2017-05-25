%{

// 필요한 함수들 정의
// 근데 Assignment2에서 썼던 헤더 파일 가져다 쓰면 될 듯?
// TODO : Syntax Tree를 만들고 Evaluate하도록 재구성!

%}

%union {
	double value;
	int index;
}
%token <value> NUM
%token <index> IDENTIFIER
%token PLUS MINUS MULTIPLY DIVIDE LEFTPARAN RIGHTPARAN ASSIGN END

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

	