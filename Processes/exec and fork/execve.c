#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	if(argc!=2){
		fprintf(stderr, "Usuage: %s <file_to_exec>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	char *nargv[] = {argv[1], "hello", "world", NULL};  // remember to terminate it by a NULL pointer
	char *nenv[] = { NULL };  // n stands for "new"

	execve(argv[1], nargv, nenv);
	perror("execve");  // execve only returns on failure
	exit(EXIT_FAILURE);
	
	return 0;
}