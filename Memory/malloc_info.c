#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <errno.h>
#include <pthread.h>

/*The program below takes up to four command-line arguments, of which the  first  three  are
   mandatory.   The  first  argument  specifies the number of threads that the program should
   create.  All of the threads, including the main thread, allocate the number of  blocks  of
   memory  specified  by  the  second  argument.  The third argument controls the size of the
   blocks to be allocated.  The main thread creates blocks of this size,  the  second  thread
   created  by  the  program  allocates blocks of twice this size, the third thread allocates
   blocks of three times this size, and so on. */


// Note here we are not freeing the space to demonstrate the use of malloc_info, but one should do that

#define exit_on_error(msg) \
	do { perror(msg); exit(EXIT_FAILURE); } while(0)

static size_t sz;
static int n_blocks, n_threads;

void *thread_start(void *id){
	int i;
	for(i=0;i<n_blocks;i++){
		if(malloc(sz*(((int)id)+2))==NULL){  // +2 ensures all threads allocate different memory sizes
			printf("Malloc error: thread %d.\n", (int)id);
			exit(EXIT_FAILURE);
		}
	}

	sleep(100);  // sleep until the main thread terminates
	return NULL;
}

int main(int argc, char const *argv[])
{
	if(argc<4){
		printf("Three agruments must be supplied.\n");
		printf("USUAGE: ./malloc_info <#threads> <#blocks> <initial block size> [sleep time]\n");
		exit(EXIT_FAILURE);
	}

	n_threads = atoi(argv[1]);
	n_blocks = atoi(argv[2]);
	sz = atoi(argv[3]);
	int sleepTime = (argc > 4) ? atoi(argv[4]) : 0;  // Note the style here

	n_threads++; // to count in the main thread as well

	int s = malloc_info(0, stdout);  // as currently implemented, the first argument is zero
	if(s==-1)
		exit_on_error("malloc_info");

	int i;

	for(i=0;i<n_threads;i++){
		pthread_t t;
		s = pthread_create(&t, NULL, thread_start, (void*)i); // we are passing i so that we can allocate blocks
																// of appropriate size
		if(s!=0){
			printf("Error occured in creating thread %d\n", i);
			exit(EXIT_FAILURE);
		}
	
		if(sleepTime>0)
			sleep(sleepTime);
	}


	for(i=0;i<n_blocks;i++){
		if(malloc(sz)==NULL){
			printf("Error: main. ENOMEM.\n");
			exit(EXIT_FAILURE);
		}
	}

	sleep(2);  // give all threads a chance to complete allocations

	printf("===================================================================\n");  // to separate the 2 outputs
	s = malloc_info(0, stdout);  
	if(s==-1)
		exit_on_error("malloc_info");

	exit(EXIT_SUCCESS);
}
