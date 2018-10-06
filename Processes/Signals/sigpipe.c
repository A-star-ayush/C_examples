#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>

// When you write to a socket after having received a RST, SIGPIPE is generated. The default action to
// terminate the process. Also, it is generated for broken pipes (pipes and FIFOs).

void sigpipe_handler(int sig) {
	write(STDOUT_FILENO, "Handler was called\n", 20);  // remember: \n counts as 1 character only
	return;
}

// error checking skipped for brevity
int main(int argc, char const *argv[])
{
	struct sigaction act;
	act.sa_handler = sigpipe_handler;
	act.sa_flags = SA_RESTART;
	sigaction(SIGPIPE, &act, NULL);

	int fd = socket(PF_INET, SOCK_STREAM, 0);
	struct sockaddr_in add;
	memset(&add, 0, sizeof(struct sockaddr_in));
	add.sin_family = AF_INET;
	add.sin_port = htons(65000);   // use nc -lvvp 65000 to create a server
	inet_aton("127.0.0.1", &add.sin_addr);

	connect(fd, (struct sockaddr*)&add, sizeof(struct sockaddr_in));

	/* Terminate the server created using nc. If you termiate it after HELLO1, the second write will simply fail
	   with connection reset and not generate a SIGPIPE. But if you terminate it before HELLO1, the first write
	   will fail with connection reset and the second write will cause a SIGPIPE to be generated. */

	getchar();
	write(fd, "HELLO1\n", 8);
	getchar();
	write(fd, "HELLO2\n", 8);

	puts("Reached Here");  // If this gets printed, it means the SIGPIPE signal was either not generated or was handled.
	close(fd);


	return 0;
}