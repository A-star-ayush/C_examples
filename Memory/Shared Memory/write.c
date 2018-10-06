#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>

// program to open existing SHM and copy string to it

#define error_handler(msg) \
	do { perror(msg); exit(EXIT_FAILURE); } while(0)

int main(int argc, char const *argv[])
{
	if(argc!=3){
		printf("Usuage: %s <name of exisitng SHM> <string to copy>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	int fd = shm_open(argv[1], O_RDWR | O_CREAT, 0777);  // the last field is left as zero since we are opening an already
									// existing SHM object so this field will simply be ignored
    // Also note there is not O_WRONLY flag since many implementations require read permission to write as well

	if(fd==-1)
		error_handler("shm_open");

	size_t sz = strlen(argv[2]);
	ftruncate(fd, sz);  // resizing the SHM to the length of the string

	char* addr = mmap(NULL, sz, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if(addr == MAP_FAILED)
		error_handler("mmap");

	close(fd);   // The file descriptor is no longer needed

	memcpy(addr, argv[2], sz);  // argv[2] is copied without the trailing '\0'

	int res = munmap(addr, sz);
	if(res == 0)
		puts("Unmapped successfully.");
	else
		puts("Encountered a problem in unmapping.");

	getchar();

	return 0;
}