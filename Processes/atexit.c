#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void after_main(void);

 // also checkout on_exit.c

int main(int argc, char const *argv[])
{
	puts("In the main prgram");
	long max = sysconf(_SC_ATEXIT_MAX);
	printf("Manimum limit for atexit: %ld\n", max);
	
	atexit(after_main);
	
	return 0;
}

void after_main(void){   // this function cannot be passed any argument
	puts("Having returned from the main program. BYE!");
}