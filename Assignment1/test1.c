/**
* Test Data 1
* Checking basic information
*/

#include <stdio.h>

int main(void) {
	int age, i;
	char initial[3] = {0};

	printf("Enter your initial (first 3 letters) : ");

	for(i = 0; i < 3; i++)
		scanf("%d", &initial[i]);
	printf("Enter your age : ");
	scanf("%d", &age);

	printf("Your name is %c%c%c\n", initial[0], initial[1], initial[2]);
	printf("and your age is %d", age);

	return 0;
}