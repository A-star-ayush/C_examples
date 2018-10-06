#include <stdio.h>


// Example of linear recursion [Single activation call to itself]

int fact(int n){
	if(n<=1)
		return 1;
	else
		return n*fact(n-1);
}

int main(int argc, char const *argv[])
{	
	printf("N: ");
	int n;
	scanf("%d",&n);	
	printf("Factorial of N: %d\n",fact(n));
	return 0;
}