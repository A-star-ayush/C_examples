#include <unistd.h>
#include <sys/mman.h>
#include <signal.h>
#include <malloc.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>


/* The program below allocates four pages of memory, makes the third of these pages read-only, and  
     then executes a loop that walks upward through the allocated region modifying bytes.*/

#define handle_error(msg) \
	do {perror(msg); exit(EXIT_FAILURE);} while(0)

char *buffer;

static void handler(int sig, siginfo_t *si, void *unused){
	printf("Got SIGSEGV at address: 0x%lx\n", (long)si->si_addr);
	exit(EXIT_FAILURE);
}

int main(int argc, char const *argv[])
{
	char *p;
	int pagesize;
	struct sigaction sa;

	sa.sa_flags = SA_SIGINFO;
	sigemptyset(&sa.sa_mask);
	sa.sa_sigaction = handler;
           
	if (sigaction(SIGSEGV, &sa, NULL) == -1)
		handle_error("sigaction");

	pagesize = sysconf(_SC_PAGE_SIZE);
	printf("Page size: %d\n", pagesize);
	if (pagesize == -1)
		handle_error("sysconf");

           /* Allocate a buffer aligned on a page boundary;
              initial protection is PROT_READ | PROT_WRITE */

	buffer = memalign(pagesize, 4 * pagesize);  // memalign is obsolete [use posix_memalign instead from now on]
	if (buffer == NULL)
		handle_error("memalign");

	printf("Start of region:        0x%lx\n", (long)buffer);  // mark the way the address is printed
	                                                        // directly %p cud also have been used

	if (mprotect(buffer + pagesize * 2, pagesize, PROT_READ) == -1)
        handle_error("mprotect");

	for (p = buffer ; ; )  // trying to write to read-only memory locations
        *(p++) = 'a';

	printf("Loop completed\n");     /* Should never happen */
	exit(EXIT_SUCCESS);
}