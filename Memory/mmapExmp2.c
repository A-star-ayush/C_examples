#include <fcntl.h>
#include <sys/uio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdio.h>

// note: to use demand-zero memory (i.e. not backed by a file) use the flag MMAP_ANON 

int main(int argc, char const *argv[])
{
	int out = open("x_wr", O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	const char* sample="Sample text";
	write(out, sample, 11);  // Note here sizeof(sample) wud give 8 and sizeof(*sample) wud give 1

	if(out != -1){
	long pageSize = sysconf(_SC_PAGESIZE);  // Note this is the size in bytes (and not in terms of ints or something)

	char *marr = mmap(NULL, pageSize, PROT_WRITE | PROT_READ, MAP_SHARED, out, 0);

	if(marr != MAP_FAILED){
		puts("Mapping succeeded.");
		int i;
		marr[0]='A';  // Now this works (By now I mean after writing something to the file pointed by the descriptor)
		for(i=1;i<15;i++)
			marr[i]='A';  // The shocking thing is that it writes only till 11 and not till 15
								// Basically when you mmaped, you asked for a constant sized array
								// so when you try to cross the boundary index, nothing happened in this case
	}

	int res = munmap(marr, pageSize);
	
	if(res == 0)
		puts("Unmapped successfully");
	}

	else
		puts("Couldn't open the file.");

	return 0;
}