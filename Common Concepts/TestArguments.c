#include <stdio.h>

int main(int argc, char const *argv[])
{
	printf("No. of arguments passed: %d.\n",argc);
	printf("The program was called with the name: %s.\n",argv[0]);

	if(argc>1)
	{
         int i=1;
		for(;i<argc;i++)
			printf("argument %d: %s.\n",i,argv[i]);
	}
	return 0;
}