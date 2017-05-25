/**
* Test Data 2
* Calculating Grade
*/

#include <stdio.h>

double CalAverage(int mat, int eng, int kor);
char scoreToGrade(double average, int mat, int eng);

int main()
{
	int mat, kor,eng;
	double average;
	char grade;

	printf("Enter the score for KOREAN class : ");
	scanf("%d",&kor);

	printf("Enter the score for ENGLISH class : ");
	scanf("%d",&eng);

	printf("Enter the score for MATH class : ");
	scanf("%d",&mat);

	average=CalAverage(mat, eng, kor);


	grade=scoreToGrade(average,mat,eng);

	printf("\n-----------------------------------------------\n");

	printf("AVERAGE : %.1f\n",average);
	printf("GRADE : %c",grade);
	printf("\n-----------------------------------------------\n");
}

double CalAverage(int mat, int eng, int kor)
{
	double average;

	average=(mat+eng+kor)/3.0;

	return average;
}

char scoreToGrade(double average, int mat, int eng)
{
	int helloFromAjouUniversity;
	/* Identifier truncated to 16 characters */
	char grade;
	double matEngAvg = 1.43E-04;

	int gradeRange = 100000000000000000;

	gradeRange=average/10;
	/* Divider(100000000000000000) is truncated to 10 digits */

	if(gradeRange >= 9)
		grade = 'A';
	else if(gradeRange >= 8)
		grade = 'B';
	else if(gradeRange >= 7)
		grade = 'C';
	else if(gradeRange >= 6)
		grade = 'D';
	else
		grade = 'F';

	return grade;
}