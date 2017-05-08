#ifndef DEFS__
#define DEFS__
#define IDENTIFIER	1
#define NUM			2

#define PLUS		3
#define MINUS		4
#define MULTIPLY	5
#define DIVIDE		6
#define ASSIGN		7

#define LEFTPARAN	8
#define RIGHTPARAN	9

#define WHITESPACE	10
#define END			11

#define REDUCE		0
#define ERROR		-1

#define DATA		20
#define OPERATOR	21
#define VARIABLE	22

#define MAXENTRY	100

typedef union {
	int index;
	double value;
}TOKEN;
#endif // !DEFS__