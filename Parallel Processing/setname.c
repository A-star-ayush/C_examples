#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>

/* The program below demonstrates the use of pthread_setname_np() and pthread_getname_np() */
// link with -pthread

#define error_exit_en(en, msg) \
	do { errno=en; perror(msg); exit(EXIT_FAILURE); } while(0)

// en in the name means error number  [Remember after the '\' above there should directly be a '\n' and nothing else]

// perror first prints the msg: and then the error (if msg is not NULL or '\0')
	// this property is used to know where the error was generated


#define NAME_LEN 16

static void* thread_start(void* arg){
	sleep(5);
	return NULL;
}

int main(int argc, char *argv[])
{
	pthread_t t;
	int s;  // we are using s as a variable to capture the return values of the function and check for errors
	char nameBuff[NAME_LEN];

	s = pthread_create(&t, NULL, thread_start, NULL);
	if(s!=0)
		error_exit_en(s, "pthread_create");

	s = pthread_getname_np(t, nameBuff, NAME_LEN);
	if(s!=0)
		error_exit_en(s, "pthread_getname_np");

	printf("Created a thread. Default name is:%s\n", nameBuff); // all the threads inherit the process name by default

	s = pthread_setname_np(t, (argc>1)? argv[1] : "SAMPLE_NAME");  // remember the length is restricted to 16
	if(s!=0)
		error_exit_en(s, "pthread_setname_np");

	sleep(2);  // sleep for 2 seconds

	s = pthread_getname_np(t, nameBuff, NAME_LEN);
	if(s!=0)
		error_exit_en(s, "pthread_getname_np");

	printf("Name of the thread now: %s\n", nameBuff);

	s = pthread_join(t, NULL);
	if(s!=0)
		error_exit_en(s, "pthread_join");

	printf("DONE\n");
	exit(EXIT_SUCCESS);  // terminates both the threads (main and the one created)
	
	return 0;
}

/*	The following shell session shows a sample run of the program:

           $ ./a.out
           Created a thread. Default name is: a.out
           Name of the thread now: THREADFOO.
           ^Z                           # Suspend the program
           [1]+  Stopped           ./a.out
           $ ps H -C a.out -o 'pid tid cmd comm'
             PID   TID CMD                         COMMAND
            5990  5990 ./a.out                     a.out
            5990  5991 ./a.out                     THREADFOO
           $ cat /proc/5990/task/5990/comm
           a.out
           $ cat /proc/5990/task/5991/comm
           THREADFOO  
*/

// cmd is CMD and comm is COMMAND [mark the difference]