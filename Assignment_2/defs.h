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

typedef struct {
	char name[100];
	double value;
}SYMBOL;
