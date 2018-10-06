#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>   // for the O_* flags
#include <sys/mman.h>  // for the mmap function
#include <sys/stat.h> // for S_I* flags

// create a SHM object with the given name and size
// link with -lrt

#define error_handler(msg) \
	do { perror(msg); exit(EXIT_FAILURE); } while(0)

int main(int argc, char const *argv[])
{
	if(argc!=3){
		printf("Usuage: %s <name> <size>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	size_t sz = atoi(argv[2]);

	int fd = shm_open(argv[1], O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
	if(fd==-1)
		error_handler("shm_open");

	ftruncate(fd, sz);  // important step because intially the SHM is zero-sized
	// or we can even write something to it to increase its size

	printf("Shared memory object successfully opened and truncated to size %ld\n",(long)sz);

	void* addr = mmap(NULL, sz, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); 

	if(addr == MAP_FAILED)
		error_handler("mmap");

	puts("SHM successfully mapped into the process's address space.");

	struct stat buf;
	fstat(fd, &buf);
	printf("Size of the mapped object is %ld\n", (long)buf.st_size);

	printf("Press any key to continue: ");
	getchar();  //  wait for other applications requiring the SHM before unlinking it 

	if(munmap(addr, sz))   // not needed since SHM objects are automatically unmapped on process termination
		error_handler("munmap"); // ignore this in subsequent files as well unless you manually want to unmap 
	
	puts("Successfully unmapped");
	
	if(shm_unlink(argv[1]))
		error_handler("shm_unlink");

	puts("Successfully unlinked");


	return 0;
}
