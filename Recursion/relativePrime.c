#include <stdio.h>

// a better way would be to perform gcd and check if it is 1
int relPrime(int x, int y){
	if(x==1 || y==1)
		return 1;
	if(x==y)   // by now x!=1 and y!=1
		return 0;
	if(x<y)
		return relPrime(x, y-x);
	else
		return relPrime(x-y, y);
}

int main(int argc, char const *argv[])
{
	int x, y;
	printf("Num1: ");
	scanf("%d",&x);
	
	printf("Num2: ");
	scanf("%d",&y);

	printf("Relative Prime: %s\n", relPrime(x, y)? "YES": "NO");
		
	return 0;
}