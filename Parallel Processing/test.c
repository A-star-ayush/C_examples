#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>  // for sleep


/* In this program, we are causing a thread to terminate a loop in the main thread*/
//link with -pthread

#define error_handler(en, msg) \
	do {perror(msg); exit(EXIT_FAILURE);} while(0)

volatile int loop_var;  // we make it volatile since its value can be accessed multiple threads
                              // although we are using a single thread in this example

void *thread_start(void *arg){
	puts("Thread has started.");
	sleep(2);  // sleep for 2 seconds
	puts("Sleeping done.");
	loop_var = 0;
	pthread_exit(NULL);
}

int main(int argc, char const *argv[])
{
	loop_var = 1;
	pthread_t t;
	int s = pthread_create(&t, NULL, thread_start, NULL);
	if(s!=0)
		error_handler(s, "pthread_create");

	while(loop_var);
	puts("Out of the while loop.");
	return 0;
}