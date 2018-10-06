#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>


int main(int argc, char const *argv[])
{
	
	if(argc>1){
		if(!mallopt(M_CHECK_ACTION, atoi(argv[1]))) {
		puts("mallopt returned with an error.");
		exit(EXIT_FAILURE);
		}
	}

	void *ptr = malloc(20);
	if(ptr==NULL){
		puts("malloc returned NULL.");
		exit(EXIT_FAILURE);
	}

	free(ptr);
	puts("First free successfully performed.");
	free(ptr);  // second time free (error)
	puts("Second free successfully performed.");

	return 0;
}

/* Sample run:

		./mallopt 0
			First free successfully performed.
			Second free successfully performed.


		./mallopt 1
			First free successfully performed.
			*** Error in `./mallopt': double free or corruption (fasttop): 0x000000000066a010 ***
			Aborted (core dumped)

		./mallopt 2
			First free successfully performed.
			Aborted (core dumped)

    and so on..  */


/* Sample run using the environment variable & without using argv[1] [MALLOC_CHECK_=0; export MALLOC_CHECK_; ./a.out]:
		
			First free successfully performed.
			Second free successfully performed.

*/