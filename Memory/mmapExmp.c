#include <fcntl.h>
#include <sys/uio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdio.h>

int main(int argc, char const *argv[])
{
	int in = open("x1", O_RDWR);   // for brevity error-checking is not implemented
	int sz = lseek(in, 0, SEEK_END) - lseek(in, 0, SEEK_SET);  // size of the file
									// or u cud have directly used fstat on in and the checked the size field of struct stat
	long pageSize = sysconf(_SC_PAGESIZE);

	while(pageSize<sz)
		pageSize+=pageSize;  // we use mmap in the chunks of pageSize for proper mapping
	// for a more efficient way to calculate this, checkout multiple.c under "Performance programming"

	char *marr = mmap(NULL, pageSize, PROT_READ | PROT_WRITE, MAP_SHARED, in, 0);  // NULL means no preffered address

	// now marr can simply be used as an character "ARRAY" !
	if(marr != MAP_FAILED){
		puts("Mapping succeeded\n");
		int i;
		/*for(i=0;i<sz;i++)   
			printf("%c",marr[i]);
		puts("");                     
		*/
		
		/*const char* replacement="xxxxx";  // these modify only the first 5 character of the file including \n
		for(i=0;i<5;i++)
			marr[i]=replacement[i];  // no change will be reflected in the file if MAP_PRIVATE 
			                           // (instead a copy will be maintained)
		puts("Replacement done\n");*/

		/*for(i=0;i<pageSize;i++)
			printf("%c",marr[i]);
		puts("");*/
	}


	int res = munmap(marr, pageSize);
	if(res == 0)
		puts("Unmapping succeeded");
	else
		puts("There was some problem in unmapping");
	
	return 0;
}