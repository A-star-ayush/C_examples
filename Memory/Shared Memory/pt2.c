#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

// this program derefrences a pointer to an int in the shared memory opened by pt1.c

// Error handling has been skipped for brevity

// Note: Again using "relative" addressing here

int main(int argc, char const *argv[])
{
	int fd = shm_open("/SHM1", O_RDONLY, 0); // opening already exisiting SHM created by pt1.c

	struct stat buf;
	fstat(fd, &buf);
	printf("Size returned: %ld\n", (long)buf.st_size);

	int* ptr = mmap(NULL, buf.st_size, PROT_READ, MAP_SHARED, fd, 0);
	close(fd);

	printf("Base address: %p\n", ptr);
	printf("Pointer offset: %d\n", *ptr);

	int* target = ptr+ *ptr;   // sematics: target = base_address + *p
	// in our example the very first element in the shared memory is the pointer

	printf("Target address: %p\n", target);
	printf("Number read: %d\n", *target);  // should be 5 on successful execution 

	munmap(ptr, buf.st_size);

	return 0;
}