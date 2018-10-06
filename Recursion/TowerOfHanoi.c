#include <stdio.h>

// Example of mulitple recursion
void print(int a, int b){
	printf("Move a disk from %d to %d.\n",a,b);
}

void move(int n, int src, int dest, int aux){
	if(n==1)
		print(src, dest); // a single disk can directly be moved  (The inductive base step)
	else
	{
		move(n-1, src, aux, dest);  // Semantics:  1) move n-1 disks from peg 1 to 3
		print(src, dest);                       // 2) move the nth disk from peg 1 to 2
		move(n-1, aux, dest, src);              // 3) move n-1 disks from peg 3 to 2
	}
}

int main(int argc, char const *argv[])
{
	printf("Enter the number of rings: ");
	int n;
	scanf("%d",&n);
	puts("Do the following:");
	move(n,1,2,3);  // we are taking the three pegs to be named 1, 2 and 3
									// and rings have to moved from 1 to 2 using 3
	return 0;
}