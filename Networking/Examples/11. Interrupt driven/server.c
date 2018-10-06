#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <fcntl.h>


// Refer to "Socket Programming in C" under Guides/C/"Network Programming" for a more details

// This code server as a simple TCP echo server which is interrupt driven
// For client : use telnet or netcat (nc) [e.g. telnet 127.0.0.1 30000]


// Drawback of using this method: the information about which fd caused the SIGIO singal is not passed to the handler.
// Therefore as seen below we had to make a global variable to hold the client socket fd. If it's just one client we can still
// manage with a global variable but what if we want to handle multiple clients? How will we know which client generated the
// signal? One way is to use select inside the signal handler to check which fd is ready. However select is not async-safe
// so avoid using it inside the handler if it's already being used elsewhere or do proper masking of signals.

int cli;

#define exit_on_error(msg) \
	do { perror(msg); exit(EXIT_FAILURE); } while(0)

#define aCast(x) (struct sockaddr*)x

int createSocket(int type, int aFm, short pNum, int bAdd, int nListen){
	struct sockaddr_in lAdd;
	int fd, rt, sz = sizeof(struct sockaddr_in);
	if(type == 1)
		fd = socket(aFm, SOCK_STREAM, 0);
	else
		fd = socket(aFm, SOCK_DGRAM, 0);
	if(fd==-1) exit_on_error("createSocket:socket");
	bzero(&lAdd, sz);
	lAdd.sin_family = aFm;
	lAdd.sin_port = htons(pNum);
	lAdd.sin_addr.s_addr = bAdd;
	rt = bind(fd, aCast(&lAdd), sz);
	if(rt==-1) exit_on_error("createSocket:bind");
	if(type==1 && nListen > 0) { rt = listen(fd, nListen);  // listen operation not supported for UDP
				  if(rt==-1) exit_on_error("createSocket:listen"); }
	return fd;
}

void sigio_handler(int sig) {
	// the handler will be called for all input events in this case (even when the client closes the connection)
	write(STDOUT_FILENO, "Handler was called\n", 20);

	char buffer[BUFSIZ];
	int rt = read(cli, buffer, BUFSIZ);
	if (rt > 0)
		write(STDOUT_FILENO, "Read: ", 7);
	
	write(STDOUT_FILENO, buffer, rt);
	write(cli, buffer, rt);
}

int main(int argc, char *argv[])
{
	{  // Handle the SIGIO singal
		struct sigaction act;
		act.sa_handler = sigio_handler;
		act.sa_flags = SA_RESTART;
		int rt = sigaction(SIGIO, &act, NULL);
		if (rt != 0)
			exit_on_error("sigaction");
	}

	int soc = createSocket(1, AF_INET, 30000, inet_addr("127.0.0.1"), 5);
	// If you enable the delivery of SIGIO on soc, the connection request will cause SIGIO to be
	// generated as well

	cli = accept(soc, NULL, NULL);
	if (cli < 0)
		exit_on_error("accept");

	{  // Enable delivery of SIGIO on I/O interrupts (note cli was used not soc)

		// ensuring that the signal will be delivered to this process
		if (fcntl(cli, F_SETOWN, getpid()) < 0)
			exit_on_error("fcntl:F_SETOWN");
		
		int arg = 1;
		if (ioctl(cli, FIOASYNC, (char*)&arg) < 0)
			exit_on_error("ioctl:FIOASYNC");
	}

	puts("Accepeted a connection");

	while(1);

	return 0;
}
