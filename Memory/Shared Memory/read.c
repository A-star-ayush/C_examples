#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>  // for STDOUT_FILENO

// program to open an exisitng SHM object and print its content to stdout

#define error_handler(msg) \
	do { perror(msg); exit(EXIT_FAILURE); } while(0)

int main(int argc, char const *argv[])
{
	if(argc!=2){
		printf("Usuage: %s <name of existing SHM>\n",  argv[0]);
		exit(EXIT_FAILURE);
	}

	int fd = shm_open(argv[1], O_RDONLY, 0);  // we do not need write permissions since we won't be making changes
	if(fd==-1)
		error_handler("shm_open");

	struct stat buf;
	fstat(fd, &buf);
	size_t sz = buf.st_size;

	char* addr = mmap(NULL, sz, PROT_READ, MAP_SHARED, fd, 0);
	if(addr == MAP_FAILED)
		error_handler("mmap");

	close(fd);  // fd is no longer required
	write(STDOUT_FILENO, addr, sz);
	puts("");

	int res = munmap(addr, sz);
	if(res == 0)
		puts("Unmapped successfully.");
	else
		puts("Encountered a problem in unmapping.");

	res = shm_unlink(argv[1]); // remove SHM object name, mark for deletion once all processes have closed

	if(res == 0)
		puts("Unlinked successfully.");
	else
		puts("Encountered a problem in unlinking.");

	return 0;
}
