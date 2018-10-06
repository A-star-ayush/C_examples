#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

// SIGUSR1 and SIGUSR2 are set aside for you to use any way you want. They are useful for inter-process communication,
// if you write a signal handler for them in the program that receives the signal.

// In this example, process 1 will ask the user for the pid of process 2 (can be inquired using ps) and then send
// a SIGUSR1 singal to it. So start process 2 before process 1.

int main(int argc, char *argv[])
{
	int pid;
	printf("Enter the pid of process 2: ");
	scanf("%d", &pid);

	// Run both process 1 & 2 from the user id so that we don't run into any permission problems
	kill(pid, SIGUSR1);
	return 0;
}
