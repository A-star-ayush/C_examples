#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
	pid_t c = fork();
	if(c==0)
	puts("Child");
	else
	puts("Parent");

	return 0;
}