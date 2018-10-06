#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
	int i;
	for(i=0;i<argc;i++){
		printf("%s\n", argv[i]);
	}

	exit(EXIT_SUCCESS);
}