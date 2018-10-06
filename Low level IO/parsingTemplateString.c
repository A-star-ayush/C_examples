#include <stdio.h>
#include <stdio_ext.h>   // needed for the macro FSETLOCKING_BYCALLER
#include <stdlib.h>    // needed for malloc
#include <printf.h>

int main(int argc, char const *argv[])
{
	__fsetlocking(stdin,FSETLOCKING_BYCALLER);  // removing the automatic stream locking on input and output
	__fsetlocking(stdout,FSETLOCKING_BYCALLER);
	
	printf("Provide a template string:\n"); 
	size_t sz=20;
	char *temp=(char*)malloc(sz+1);
	getline(&temp,&sz,stdin);

	int argtype[10]; // the functions only count the legal type specifiers
	int n=parse_printf_format(temp,10,argtype);  // this function is useful for the interpretors that provide a
													// printf like interface
	if(n>10)
		puts("More than 10 arguments were provided. Thus no analysis performed.");
	else  
	{		// the information about the arguments are encoded using various 'PA_' macros inside the argtype array
		int i;
		for(i=0;i<n;i++){
			switch(argtype[i]&~PA_FLAG_MASK){ // helps us recognize the base type. If ~ is not used, we get flag bits.
				case PA_INT: printf("Type is int.\n");
							 break;
				case PA_CHAR: printf("Type is int, cast to char.\n");
							 break;
				case PA_STRING: printf("Type is char*, a null terminated string.\n");
							 break;
				case PA_FLOAT: printf("Type is float.\n");
							 break;
				default: printf("Not int, char, string or float.\n");   // there are many more macros we didn't 
																					// consider here
			}	
		}	
	}



	return 0;
}