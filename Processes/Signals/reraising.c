#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <termios.h>


// Turns off terminal echo
// Upon receiving a SIGKILL singal, turn terminal echo back on, specify the default handler and reraise the singal.

void turnEchoOnSignal(int sig) {
	struct termios old, new;
	tcgetattr(STDIN_FILENO, &old);
	new = old;
	new.c_lflag |= ECHO;
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &new);

	// specify the default action for the signal
	struct sigaction act;
	act.sa_handler = SIG_DFL;
	act.sa_flags = SA_RESTART;
	sigaction(SIGINT, &act, NULL);
	// reraise the signal
	kill(getpid(), SIGINT);
}

int main(int argc, char *argv[])
{
	printf("Enter password: ");  // not storing anything since it's just an example

	// Skipping error-checking for brevity
	struct termios old, new;
	tcgetattr(STDIN_FILENO, &old);
	new = old;
	new.c_lflag &= ~ECHO;
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &new);
	// TCSAFLUSH - apply the changes after all output to the specified fd has been transmitted 
	// and all input that has been received but not read is discarded

	struct sigaction act;
	act.sa_handler = turnEchoOnSignal;
	act.sa_flags = SA_RESTART;
	sigaction(SIGINT, &act, NULL);

	char* line = NULL; size_t n = 0;
	getline(&line, &n, stdin);
	
	{
		struct termios old, new;
		tcgetattr(STDIN_FILENO, &old);
		new = old;
		new.c_lflag |= ECHO;
		tcsetattr(STDIN_FILENO, TCSAFLUSH, &new);	
	}

	return 0;
}