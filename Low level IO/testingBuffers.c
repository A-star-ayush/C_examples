#include <stdio.h>
#include <stdio_ext.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
	char inputBuffer[BUFSIZ]; // BUFSIZ is a macro that tells the optimum size of chunk for I/O [8192 on my laptop]
 							// here we are using unbuffered input so setting input buffer to null will do no harm
	setvbuf(stdin,inputBuffer,_IONBF,BUFSIZ);  // _IONBF means unbuffered 

	char outputBuffer[BUFSIZ];
	setvbuf(stdout,outputBuffer,_IONBF,BUFSIZ);  // making the output unbuffered too  

	__fsetlocking(stdin,FSETLOCKING_BYCALLER);   // removing the locks for faster input and output
	__fsetlocking(stdout,FSETLOCKING_BYCALLER);


	size_t n=10;
	char *str=(char*)malloc(n+1);
	int i=5;
	while(i--){
		getdelim(&str,&n,' ',stdin);  // remember the single quotes
		// __fpurge(stdin);   // it has no effect since we are using unbuffered input and output
		printf("%s\n",str);
	}

	return 0;
}