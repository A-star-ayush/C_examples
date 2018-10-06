#include <stdio.h>
#include <fcntl.h>
#include <sys/uio.h>
#include <sys/mman.h>
#include <unistd.h>

// use -lrt while compiling  [necessary for shm_open]

int main(int argc, char const *argv[])
{
	int dataSrc=shm_open("/DATA1", O_RDWR, S_IRUSR | S_IWUSR);  // the last argument can be specified as 0 when 
																// opening an existing SHM object
	
	int arr[10];
	size_t n=read(dataSrc, arr, 4);
	printf("Number read: %d\n",arr[0]);  // If the link is closed from the other file, it simply reads the number
											// from that memory location
	return 0;
}