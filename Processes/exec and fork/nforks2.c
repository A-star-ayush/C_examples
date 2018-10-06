#include <stdio.h>
#include <sys/types.h>
#include <unistd.h> 
#define N 2

int main(){

  	int i;
  	for (i = 1; i <= N; i++) fork();
  	printf("Hey! from %d, son of %d\n", getpid(), getppid());  // for N = 2, it gets printed 4 times

	return 0;
}

/*
	Total no of children created: 2^N - 1  .. 3 here; with the parent process, it makes a total of 4 processes
	To see why it is so, check saved webpages under C for "I have a C program, which loops 'n' times calling fork each loop..."
*/