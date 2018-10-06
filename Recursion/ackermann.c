#include <stdio.h>

// Note that the Ackermann function grows very fast 
// it is a non-elementary function: A(x, x) grows faster than any tower of exponentials 22 ... 2x. 

int ack(int m, int n){
	if(m==0)
		return n+1;
	if(n==0)
		return ack(m-1,1);
	else
		return ack(m-1,ack(m,n-1));
}

int main(int argc, char const *argv[])
{
	int n,m;

	printf("M: ");
	scanf("%d",&m);
	printf("N: ");
	scanf("%d",&n);

	printf("Ackermann Value: %d\n",ack(m,n));
	return 0;
}