#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>  // for sysconf

// this program stores a pointer to an int in the shared memory
// the file pt2.c reads the value of the integer via the pointer

// Error handling has been skipped for brevity

// Note: when declaring pointers in shared memory always use "relative addresses" and not absolute ones
// since while mapping the SHM object in the caller's address space, mmap may map the SHM object to different
// addresses in every process.

// Refer to the pdf mentioned in "Checkout" for more clarity

int main(int argc, char const *argv[])
{
	int fd = shm_open("/SHM1", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR); // remember the O_CREAT

	long pageSize = sysconf(_SC_PAGESIZE);
	ftruncate(fd, pageSize);

	int* ptr = mmap(NULL, pageSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	close(fd);

	int* base_address = ptr;
	printf("Base address: %p\n", base_address);

	int* pointer_location = ptr;

	ptr+=1;  // to store the pointer offset [although it should have been 8 bytes but even 4 will do here]
	*ptr = 4; ptr++;
	*ptr = 5; // no ptr++ so that ptr is now pointing to 5
	*pointer_location = ptr - base_address;  // semantics: *p = target - base_address
	// in our example ptr currently points to the target
	printf("Target Adress: %p\n", ptr);
	
	munmap(base_address, pageSize);

	getchar();  // lets wait till pt2.c is setup

	shm_unlink("/SHM1");
	
	return 0;
}