#include <stdio.h>


int sum(int a, int b){
	if(b==0)
		return a;
	else
		return 1+sum(a,b-1);
}

int main(int argc, char const *argv[])
{
	int a,b;
	printf("a b: ");
	scanf("%d %d",&a,&b);
	printf("Sum: %d\n",sum(a,b));
	return 0;
}