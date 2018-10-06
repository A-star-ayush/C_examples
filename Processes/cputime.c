#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

/*The program below creates a thread and then uses clock_gettime() to  retrieve  the  total
    process  CPU time, and the per-thread CPU time consumed by the two threads (main and a pthread).*/


// link with -pthread

#define handle_error(msg) \
	do { perror(msg); exit(EXIT_FAILURE);} while(0)   // mark no semicolon after while because that
															// will be included when it is called
#define handle_error_en(en, msg) \
	do { errno=en; perror(msg); exit(EXIT_FAILURE); } while(0)

// we have these two definitions for handling error beacuse not all functions set the errno [only those that
// return -1 and some other functions does so]. Therefore in the second handler we first set the errno
// and then use perror to print the error message  

static void* thread_start(void *arg){
	puts("Subthread starting infinite loop");
	for(;;)
		continue;
	return 0;   
}

static void *pclock(char *msg, clockid_t cid){
	struct timespec ts;
	printf("%s",msg);
	if(clock_gettime(cid, &ts)==-1)
		handle_error("clock_gettime");  // program will also be exited if error occurs therefore we need not use else
	printf("%4ld.%03ld\n", ts.tv_sec, ts.tv_nsec / 1000000);   // converting nano seconds to micro seconds
	return 0;
}


int main(int argc, char const *argv[])
{
	pthread_t t;
	clockid_t cid;

	int i,s;

	s = pthread_create(&t, NULL, thread_start, NULL);
	if(s!=0)
		handle_error_en(s, "pthread_create");

	puts("Main thread sleeping");
	sleep(1);
	puts("Main thread consuming some CPU time...");
	for(i=0;i<2000000;i++)
		getppid();  // getppid returns the process ID of the parent whereas getpid returns the process ID 
									// of the calling process

	pclock("Process total CPU time: ", CLOCK_PROCESS_CPUTIME_ID);  //macro for the process' clock id

	s = pthread_getcpuclockid(t, &cid);
	if(s!=0)
		handle_error_en(s, "pthread_getcpuclockid");

	pclock("Thread 1 CPU time: ", cid);

	exit(EXIT_SUCCESS);   // terminates both the threads

	return 0;
}