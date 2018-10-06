#include "seqnum.h"

// Error checking skipped for brevity
// link with -pthread and -lrt

int main(int argc, char const *argv[])
{
	struct shm_buf* ptr;
	
	shm_unlink("/SHM1");  // closing any previously opened SHM object by the same name.

	int fd = shm_open("/SHM1", O_CREAT | O_EXCL| O_RDWR, S_IRUSR | S_IWUSR);

	const size_t sz = sizeof(struct shm_buf);

	ftruncate(fd, sz);

	ptr = mmap(NULL, sz, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	close(fd);

	/* Do not confuse yourself over the pointer pointee relationship here. mmap here maps a region of shared memory
	   and returns the pointer to it (so the pointee do exist). Now thing is that the pointee is raw memory ,i.e., it 
	   can be interpreted in any manner we want. Here we interpret it to contain an object of type struct shm_buf.
	   See "MARKED EXAMPLE" in test5.c for more clarity. */

	sem_init(&ptr->sem, 1, 1);  // 2nd argument specifies that the semaphore is shared between processes
	 // the 3rd argument is the initial value of the semaphore. We will be using this semaphore as a binary one.
	
	if(argc>1) // otherwise the newly extended bytes are initialised to zero by ftruncate
		ptr->seqnum = atoi(argv[1]);

	munmap(ptr, sz);

	// left out shm_unlink on puropose. Later can be deleted using rm [/dev/shm/<the name u have given>]
	
	return 0;
}