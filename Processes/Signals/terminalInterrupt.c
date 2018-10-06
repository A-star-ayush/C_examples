#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

// Use Ctrl + C to generate the interrupt from terminal

void interruptHandler(int sig)
{
	char message[] = { "\nThe handler was invoked\n" };
	write(STDOUT_FILENO, message, sizeof(message));

	// If signals haven't been masked where is a possibility of inconsistency due to interruption, use only async-safe functions
	// inside a signal handler. Therefore printf was not used. Although it wouldn't have been a problem here since no std output
	// was used in the main function.
}

int main(int argc, char* argv[])
{
	struct sigaction act;
	act.sa_handler = interruptHandler;
	act.sa_flags = SA_RESTART;   // is set so that functions such as open, read, write resume after being interrupted
								 // If you are not setting this flag, you may want to look at TEMP_FAILURE_RETRY macro
	int rt = sigaction(SIGINT, &act, NULL);
	if (rt != 0) {
		perror("sigaction");
		exit(EXIT_FAILURE);
	}
	
	pause(); // causes the calling process to sleep until a signal is delivered
	return 0;
}