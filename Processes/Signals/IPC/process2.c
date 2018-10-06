#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>


// process2 specifies a handler for SIGUSR1 whose default action is to terminate the process.

void siguser_handler(int sig) {
	write(STDOUT_FILENO, "The handler was called\n", 23);
}

int main(int argc, char *argv[])
{
	struct sigaction act;
	act.sa_handler = siguser_handler;
	act.sa_flags = SA_RESTART;
	sigaction(SIGUSR1, &act, NULL);

	printf("pid: %d\n", getpid());

	pause();

	return 0;
}
