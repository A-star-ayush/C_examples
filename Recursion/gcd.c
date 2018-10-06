#include <stdio.h>

int gcd(int a, int b){
	if(a<b) return gcd(b,a);

	int r=a%b;
	if(r==0) return b;
	
	else return gcd(b,r);
}

int main(int argc, char const *argv[])
{
	printf("Num1 Num2: ");
	int a, b;
	scanf("%d %d",&a,&b);
	printf("GCD: %d\n",gcd(a,b));
	return 0;
}