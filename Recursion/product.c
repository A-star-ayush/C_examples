#include <stdio.h>


int product(int a, int b){
	if(b==0)
		return 0;
	else
		return a+product(a,b-1);
}

int main(int argc, char const *argv[])
{
	int a,b;
	printf("a b: ");
	scanf("%d %d", &a, &b);
	printf("Product: %d\n",product(a,b));
	return 0;
}