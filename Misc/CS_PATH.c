#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

int main(int argc, char const *argv[])
{
	size_t x;
	x = confstr(_CS_PATH, NULL, (size_t)0);  // in x is stored the size of the buffer needed to store the variable value

	char* PATH;
	PATH = malloc(x);   // allocating the proper size for the buffer

	x = confstr(_CS_PATH, PATH, x);

	if(errno == EINVAL){ 
		printf("Invalid Argument to confstr command.\n");
		exit(EXIT_FAILURE);
	}


	printf("Path to standard utilities-> %s\n", (x==0) ? "No value found" : PATH);

	return 0;
}