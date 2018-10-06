#include <sys/stat.h> // needed for the permission macros
#include <fcntl.h>  // needed for the open function
#include <unistd.h> // needed for the close function and the write function
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
	int testFile=open("test", O_WRONLY| O_CREAT| O_EXCL, S_IRWXU);  // format: open(filename, flags, modes) 
																	// S_IRWXU read write execute rights for the user 
	if(testFile==-1)
		{ printf("There was some error while opening the file. The program is exiting.\n"); exit(1); }

	else
		puts("Everything went fine.");

	// the second time onwards u run the program the error message gets displayed
	    // beacuse both the flags O_CREAT and O_EXCL are set which protects any exisiting
	         // file (created in the first run) to be clobbered

	return 0;
}