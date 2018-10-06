#include <stdio.h>
#include <stdlib.h>  // for the exit function
#include <unistd.h> // for the sleep function


/* Reads "testfile" and prints the numbers in it. Then sleeps for 5 seconds. During that time change the content of testfile
	and the program will again resume to print having cleared the eof flag. */

int main(int argc, char const *argv[])
{
	FILE* in = fopen("testfile", "r");
	if(in==NULL){
		puts("Could not find the input file (by the name testfile). Exiting.");
		exit(EXIT_FAILURE);
	}

	int a;
	while(1){
		if(fscanf(in,"%d",&a)==EOF){
			puts("End of file reached. Sleeping for 5 seconds and clearing the flags.");
			sleep(5);
			clearerr(in);
			fscanf(in,"%d",&a);
			if(feof(in)){
				puts("No input even after 5 seconds. Exiting.");
				exit(EXIT_SUCCESS);
			}
		}
		printf("Read %d\n", a);
	}

	return 0;
}