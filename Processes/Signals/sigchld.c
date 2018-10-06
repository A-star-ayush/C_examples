#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

void reapChild(int sig) {
	char message[] = { "The reaper was called upon\n" };
	write(STDOUT_FILENO, message, sizeof(message));
	
	wait(NULL);
}

int main(int argc, char *argv[])
{
	struct sigaction act;
	act.sa_handler = reapChild;
	act.sa_flags = SA_RESTART | SA_NOCLDSTOP;  // SA_NOCLDSTOP - generate SIGCHLD only when child terminates and not stops.
	sigaction(SIGCHLD, &act, NULL);

	switch(fork()) {
		case -1 : perror("fork"); break;
		case 0 : sleep(2); exit(EXIT_SUCCESS);  // the child will exit after 2 seconds
		default : pause();
	}

	// The advantage of using handling SIGCHLD instead of just calling wait here is that the parent can do something
	// else meanwhile (in place of the pause function used above) + its a quick way to avoid zombie processes

	return 0;
}