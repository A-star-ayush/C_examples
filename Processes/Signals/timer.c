#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <string.h>

// link with -lrt

#define err_exit(msg) \
	do { perror(msg); exit(EXIT_FAILURE); } while(0)

/*void timerFired(int sig) {
	char message[] = { "The timer fired\n" };
	write(STDOUT_FILENO, message, sizeof(message));
}*/

/*void printSigInfo(siginfo_t* si) {
	timer_t* tidp;
	int rt;
	tidp = si->si_value.sival_ptr;
	printf("sival_ptr = %p ", si->si_value.sival_ptr);
	printf("*sival_ptr = 0x%lx\n", (long)*tidp);
	rt = timer_getoverrun(*tidp);
	if (rt == -1) err_exit("timer_getoverrun");
	else printf("overrun count = %d\n", rt);
}

void handler(int sig, siginfo_t* si, void* uc) {
	printf("Caught signal %d\n", sig);  // use of printf instead a singal handler is unsafe. Used here just for example.
	printSigInfo(si);
	signal(sig, SIG_IGN);
}*/

int main(int argc, char* argv[])
{
	/*  // Simple program with a timer that initially fires after 2 seconds and then after every second

	struct sigevent sevp;
	sevp.sigev_notify = SIGEV_SIGNAL;
	sevp.sigev_signo = SIGALRM;
	timer_t id;

	int rt = timer_create(CLOCK_MONOTONIC, &sevp, &id);
	if (rt != 0) err_exit("timer_create");

	struct sigaction act;
	act.sa_handler = timerFired;
	act.sa_flags = SA_RESTART; 
	rt = sigaction(SIGALRM, &act, NULL);
	if (rt != 0) err_exit("sigaction");

	struct itimerspec new_val;
	memset(&new_val, 0, sizeof(new_val));  // will set the nanoseconds fields to zero
	new_val.it_interval.tv_sec = 1;
	new_val.it_value.tv_sec = 2;
	rt = timer_settime(id, 0, &new_val, NULL);
	if (rt != 0) err_exit("timer_settime");

	pause();*/




	/*  // 2 arguments, a sleep period in seconds and a timer frequency in nanoseconds. The sets up the timer, blocks the signal
		// and then sleeps for the time specified. Wakes up and unblocks the signal. The signal handler will be invoked if the timer
		// expired atleast once during the sleep period. The program will however terminate after one invocation of the handler since in
		// the handler, we set to ignore any more singals for that type.

		// Sample Run: ./a.out 1 100

	timer_t timerid;
	struct sigevent sev;
	struct itimerspec its;
	struct sigaction sa;
	long long freq_nanosecs;
	sigset_t mask;

	if (argc != 3) {
		fprintf(stderr, "Usage : %s <sleep-sec> <freq_nanosecs>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	// The range of real-time signals is defined by the macros SIGRTMIN and SIGRTMAX.

	// Establish handler for timer signal
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = handler;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGRTMIN, &sa, NULL) == -1)
		err_exit("sigaction");

	// Block timer signal temporarily
	printf("Blocking signal %d\n", SIGRTMIN);
	sigemptyset(&mask);
	sigaddset(&mask, SIGRTMIN);
	if (sigprocmask(SIG_SETMASK, &mask, NULL) == -1)
		err_exit("sigprocmask");

	// Create the timer
	sev.sigev_notify = SIGEV_SIGNAL;
	sev.sigev_signo = SIGRTMIN;
	sev.sigev_value.sival_ptr = &timerid;
	if (timer_create(CLOCK_REALTIME, &sev, &timerid) == -1)
		err_exit("timer_create");

	printf("Timer ID is 0x%lx\n", (long)timerid);

	// Start the timer
	freq_nanosecs = atoll(argv[2]);
    its.it_value.tv_sec = freq_nanosecs / 1000000000;
    its.it_value.tv_nsec = freq_nanosecs % 1000000000;
    its.it_interval.tv_sec = its.it_value.tv_sec;
    its.it_interval.tv_nsec = its.it_value.tv_nsec;

    if (timer_settime(timerid, 0, &its, NULL) == -1)
        err_exit("timer_settime");

    // Sleep for a while; meanwhile, the timer may expire multiple times

	printf("Sleeping for %d seconds\n", atoi(argv[1]));
	sleep(atoi(argv[1]));

	// Unlock the timer signal, so that timer notification can be delivered

	printf("Unblocking signal %d\n", SIGRTMIN);
	if (sigprocmask(SIG_UNBLOCK, &mask, NULL) == -1)
               err_exit("sigprocmask");*/

	return 0;
}