#include <stdio.h>

int main(void) {
	int 000num = 012345; 		/* Leading Zero in int type error */
	char _correctChar = "C';	/* Quotation mark mismatch error */
	double wrongDouble = 1.345R+10;

	/* Error on not including backslash for escape sequence */
	printf("Mike said "Hello World"");
}