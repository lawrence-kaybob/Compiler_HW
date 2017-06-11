
// Defines the following definitions, if it hasn't been defined
#ifndef DEFS__
#define DEFS__
// Token value decalarations for lexical analysis
/*
#define IDENTIFIER	1
#define NUM			2

#define PLUS		3
#define MINUS		4
#define MULTIPLY	5
#define DIVIDE		6
#define ASSIGN		7

#define LEFTPARAN	8
#define RIGHTPARAN	9

#define END			11
*/
#define WHITESPACE	10



#define REDUCE		0
#define ERROR		-1

// Token for distinguishing type
// of data.
#define DATA		20	// As an number literal
#define OPERATOR	21	// As an operator
#define VARIABLE	22	// As an variable from symbol table
#define CONTROL		23

#define MAXENTRY	100	// Number of maximum entry

/**
 * <TOKEN Union>
 * Union declaration for passing the actual value of number literal,
 * or an variable index of symbol table
 *
typedef union {
	int index;
	double value;
}TOKEN;
*/
#endif // !DEFS__