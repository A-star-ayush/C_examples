#include <stdio.h>
#include <fcntl.h>
#include  <stdlib.h>

#define exit_on_error(msg) \
	do { perror(msg); exit(EXIT_FAILURE); } while(0)

int main(int argc, char const *argv[])
{	
	char msg[BUFSIZ];

	/*Opening a descriptor to FIFO for reading*/
	int in = open("test_fifo", O_RDONLY);
	if(in==-1) exit_on_error("open");

	/*Reading from the FIFO*/
	while(1){
		int rt = read(in, msg, BUFSIZ);  // will block until the writing end is closed even after all that 
		if(rt==-1) exit_on_error("read");	// was written is read
		if(rt==0) break;					// one can programme a non-blocking version ORing O_NONBLOCK in open
		printf("%s\n", msg); 
	}

	return 0;
}