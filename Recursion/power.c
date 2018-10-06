#include <stdio.h>

int power(int p, int x){
	if(x==0)
		return 1;
	else
		return p*power(p,x-1);
}

int main(int argc, char const *argv[])
{
	printf("Enter p and x: ");
	int p,x;
	scanf("%d %d",&p,&x);
	printf("Result: %d\n",power(p,x));
	return 0;
}