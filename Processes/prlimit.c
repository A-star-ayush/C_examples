#define _GNU_SOURCE              // feature macro for prlimit
#define _FILE_OFFSET_BITS 64     // feature macro for prlimit
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/resource.h>

#define error_exit(msg) \
	do {perror(msg); exit(EXIT_FAILURE);} while(0)

int main(int argc, char *argv[])
{	
	struct rlimit Old, New;

	struct rlimit *newp;

	pid_t pid;

	if(!(argc==2 || argc==4))
		fprintf(stderr, "Usage: %s <pid> [<new-soft-limit> <new-hard-limit>]\n", argv[0]);

	pid = atoi(argv[1]);
	newp = NULL;  // note the use of this pointer in this program

	if (argc==4){  // meaning new limits are provided
		New.rlim_cur = atoi(argv[2]);  // atoi - anything to integer
		New.rlim_max = atoi(argv[3]);
		newp = &New;
	}

	// for RLIMIT_CPU, checkout "rlimit.docx"

	if(prlimit(pid, RLIMIT_CPU, newp, &Old)==-1)  // here if argc would have been 2, newp would be NULL
		error_exit("prlimit");

	printf("Previous limits -> soft: %lld hard: %lld\n", (long long)Old.rlim_cur, (long long)Old.rlim_max);

	if(prlimit(pid, RLIMIT_CPU, NULL, &Old));
	printf("New limits -> soft: %lld hard: %lld\n", (long long)Old.rlim_cur, (long long)Old.rlim_max);


	if(pid==0)
		for(;;);   // a sample infinite loop that will help us see the effect
	// note we didn't use something like getchar() to stall the time, because that doesn't get counted as
			// CPU time
	
	exit(EXIT_SUCCESS);  

	return 0;
}

// Sample run
// ./prlimit 0 5 10   .. to quit this very program after it consumes 5 seconds of the CPU time