
// Defines the following definitions, if it hasn't been defined
#ifndef DEFS__
#define DEFS__
// Token value decalarations for lexical analysis

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

#endif // !DEFS__