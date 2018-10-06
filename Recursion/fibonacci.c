#include <stdio.h>


// Example of binary recursion [calling itslef twice]

int fib(n){
	if(n==0)
		return 0;
	if(n==1)
		return 1;
	else
		return fib(n-1)+fib(n-2);
}

int main(int argc, char const *argv[])
{
	printf("N: ");
	int n;
	scanf("%d",&n);
	printf("Nth fibonacci number: %d\n",fib(n));
	return 0;
}