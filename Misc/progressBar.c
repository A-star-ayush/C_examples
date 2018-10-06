#include <stdio.h>
#include <unistd.h>

int doSomething(int p){ usleep(100000); return p+1; }  // a dummy function that causes some delay

#define WIDTH 50

int main(int argc, char const *argv[])
{	
	puts("Process started");
	int progress = 0;
	float quantum = 100.0/WIDTH;
	while(progress<=100){
		int i;
		printf("[");
		for(i=0;i<(int)progress/quantum;++i) printf("#");
		for(;i<WIDTH;++i) printf(" ");
		printf("] %d\n", progress);
		progress = doSomething(progress);

		/* Clearing the line just printed using ANSI escape codes*/
		printf("\033[F\033[J");
	}	
	puts("Process completed");

	return 0;
}

#undef WIDTH