#include <stdio.h>
#include <stdlib.h>  // for exit
#include <semaphore.h>
#include <signal.h>
#include <time.h>
#include <errno.h>  // for errnos'
#include <unistd.h>  // for _exit 

// link with -pthread

/*The (somewhat trivial) program shown below operates on an unnamed semaphore. The program expects two command-line 
arguments. The first argument specifies a seconds value that is used to set an alarm timer to generate a SIGALRM signal. 
This handler performs a sem_post to increment the semaphore that is being waited on in main() using sem_timedwait(). 
The second command-line argument specifies the length of the timeout, in seconds, for sem_timedwait(). */


#define error_handler(msg) \
	do { perror(msg); exit(EXIT_FAILURE); } while(0)

sem_t sem;   // the global unnamed semaphore 

static void handler(int sig){
	puts("sem_post() from handler");
	if(sem_post(&sem) == -1){
		puts("sem_post() failed");
		_exit(EXIT_FAILURE);
	}
}

int main(int argc, char const *argv[])
{
	if(argc!=3){
		fprintf(stderr, "Usuage: %s <alarm_time> <timeout_time>\n",argv[0]);
		_exit(EXIT_FAILURE);
	}

	struct timespec ts;
	struct sigaction sa;

	if(sem_init(&sem, 0, 0)==-1)
		error_handler("sem_init");

	/* Establishing SIGALRM handler now */

	sa.sa_handler = handler;
	sigemptyset(&sa.sa_mask);  // clears the set, i.e, excludes all the signals from the set
	sa.sa_flags = 0;

	if(sigaction(SIGALRM, &sa, NULL)==-1)
		error_handler("sigaction");

	alarm(atoi(argv[1]));  // sets an alarm clock for the delivery of a signal

	/* Calculating relative time interval as current time + number of seconds given in argv[2] */

	if(clock_gettime(CLOCK_REALTIME, &ts)==-1)  // using system wide call that maintains the real time
		error_handler("clock_getime");

	ts.tv_sec += atoi(argv[2]);

	puts("main about to call sem_timedout()");

	int s;
	while((s=sem_timedwait(&sem, &ts))==-1 && errno==EINTR)
		continue;

	if(s==-1){
		if(errno == ETIMEDOUT)
			puts("sem_timedwait() timed out");
		else
			perror("sem_timedwait");
	}

	else
		puts("sem_timedwait succeeded");
    
    exit((s==0) ? EXIT_SUCCESS: EXIT_FAILURE);
    return 0;
}