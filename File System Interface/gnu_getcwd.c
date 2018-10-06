#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

// This file describes a way to implement the GNU style getcwd functionality [support for getcwd(NULL,0)]

void *xmalloc(size_t nbytes){   // xmalloc is not a library function but is a customary name used in many
	void* ptr = malloc(nbytes);              // GNU library softwares
	if(ptr==NULL){
		printf("Memory exhausted");
		exit(EXIT_FAILURE);
	}
	return ptr;
}

char* gnu_cwd(){
	size_t sz = 100;   
	while(1){
		char *buffer = (char*)xmalloc(sz);
		if(getcwd(buffer, sz) == buffer)  // Mark the way the result is checked
			return buffer;
		free(buffer);
		if(errno!=ERANGE)
			return 0;
		sz*=2;
	}
}

int main(int argc, char const *argv[])
{
	char* cwd = gnu_cwd();
	printf("Current working directory: %s\n",cwd); 	
	return 0;
}