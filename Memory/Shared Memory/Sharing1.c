#include <stdio.h>
#include <fcntl.h>  // defines the O_* flags
#include <sys/uio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>

// use -lrt while compiling  [necessary for shm_open]

int main(int argc, char const *argv[])
{
	int dataSrc = shm_open("/DATA1", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);  //  remember to use O_CREAT when
																					// creating it for the first time
	// shm_open opens existing shared memory (SHM) object, or create and open new SHM object
    // remember the mode constants (permission bits) can also be specified as that for chmod (like 777 and so on)

	int val[4]={2,10,7,9};
	write(dataSrc, val, 4*4);  // size of int - 4 bytes; cud have used sizeof(int)
	
	long pageSize = sysconf(_SC_PAGESIZE);  // sysconf has got many other variables as well
	
	int* marr = mmap(NULL, pageSize, PROT_READ | PROT_WRITE, MAP_SHARED, dataSrc, 0);  // mark the use of int* 
													// to catch the address since we are dealing with integers here
	// The use of mmap above maps the SHM object into the caller's address space
	// The PROT_* above must be consistent with the SHM object permissions specified during shm_open
	
	if(marr != MAP_FAILED)
	{
		int i;
		for(i=0;i<4;i++)
			printf("Value at address %p: %d \n",marr+i,marr[i]);		
	}
	
	getchar();  // mark getchar and not getch

	int res = munmap(marr, pageSize);
	if(res == 0)
		puts("Unmapped successfully.");
	else
		puts("Encountered a problem in unmapping.");

	res = shm_unlink("/DATA1"); // remove SHM object name, mark for deletion once all processes have closed

	if(res == 0)
		puts("Unlinked successfully.");
	else
		puts("Encountered a problem in unlinking.");
	

	return 0;
}