#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#define exit_on_error(msg) \
	do { perror(msg); exit(EXIT_FAILURE); } while(0)

int main(int argc, char const *argv[])
{
	if(mkfifo("test_fifo", 0777)==-1)  // the 2nd argument sets the permission bits
		exit_on_error("mkfifo");

	/* Opening a descriptor to FIFO for writing*/
	int out = open("test_fifo", O_WRONLY);
	if(out==-1) exit_on_error("open");

	/* Writing something to the FIFO */
	const char* msg = "HelloWorld";
	if(write(out, "Hello World!", 11)==-1) exit_on_error("write");  // write will block until the reading end is opened

	sleep(10);  // giving the reading application some time to execute
	if(remove("test_fifo")==-1) exit_on_error("remove");  // removing the fifo entry from the file system

	return 0;
}