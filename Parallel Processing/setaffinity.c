#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h> // for the exit function
#include <pthread.h>
#include <errno.h>

/*  In the following program, the main thread uses pthread_setaffinity_np()  to  set  its  CPU
    affinity  mask  to include CPUs 0 to 7 (which may not all be available on the system), and
    then calls pthread_getaffinity_np() to check  the  resulting  CPU  affinity  mask  of  the
	thread. */

// link with -pthread
    
#define handle_error_en(en, msg) \
    do { errno=en; perror(msg); exit(EXIT_FAILURE);} while(0)  // perror prints the error message
    												// mark the way the macro has used the do while wrapper

int main(int argc, char *argv[])
{
	pthread_t t = pthread_self();

	cpu_set_t cpuset;
	CPU_ZERO(&cpuset); // clears set so that it contains no CPUs

	int j;
	for(j=0;j<8;j++)  // setting affinity mask to include CPUs 0-7
		CPU_SET(j, &cpuset);

	int rt = pthread_setaffinity_np(t, sizeof(cpu_set_t), &cpuset);
	if(rt!=0)
		handle_error_en(rt, "pthread_setaffinity_np");

	rt = pthread_getaffinity_np(t, sizeof(cpu_set_t), &cpuset);  // checking the actual affinity mask
	printf("Set returned by pthread_getaffinity_np contained: \n");
	for(j=0; j<CPU_SETSIZE; j++)
		if(CPU_ISSET(j, &cpuset))
			printf("CPU%d\n", j);

	return 0;
}