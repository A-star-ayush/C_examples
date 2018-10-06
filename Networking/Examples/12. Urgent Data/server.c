#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>


// Streams with connections permit out-of-band data that is delivered with higher priority than ordinary data. 
// Typically the reason for sending out-of-band data is to send notice of an exceptional condition. However, the use
// of urgent data mechanism is discouraged due to the action of middle box (RFC 6093)

// Instead of handling SIGURG, you can alternately test for pending out-of-band data, or wait until there is out-of-band data, 
// using the select function; it can wait for an exceptional condition on the socket.

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

int cli;  // global beacuse the singal handler is not provided the fd that caused the singal


/* Notification of out-of-band data (whether with SIGURG or with select) indicates that out-of-band data are on the way; 
the data may not actually arrive until later. If you try to read the out-of-band data before it arrives, recv fails 
with an EWOULDBLOCK error. */

/* Sending out-of-band data automatically places a “mark” in the stream of ordinary data, showing where in the sequence 
the out-of-band data “would have been”. This is useful when the meaning of out-of-band data is “cancel everything 
sent so far”.*/

void urgent_data_handler(int sig) {
	char buf[BUFSIZ];
	while(1){
		int rt = recv(cli, buf, BUFSIZ, MSG_OOB);
		if (rt > 0) {
			strcat(buf, " :out-of-band\n");
			write(STDOUT_FILENO, buf, rt + 14);
			break;
		}
		else if (rt < 0) {
			int atmark;
			rt = ioctl(cli, SIOCATMARK, &atmark);
			if (rt < 0)
				exit_on_error("ioctl");
			if (atmark) {  // if atmark is non zero, socket's read pointer had reached the "mark"
				sleep(1);
				continue;  // At "mark" skipping past ordinary data won't help, so just wait a while
			}
			// Otherwise read ordinary data. This is guaranteed to not read past the mark if it starts before the mark.
			rt = read(cli, buf, BUFSIZ);
			if (rt < 0)
				exit_on_error("read");
			else if (rt > 0) {
				strcat(buf, " :ordinary\n");
				write(STDOUT_FILENO, buf, rt + 11);
			}
		}
	}
}

int main(int argc, char *argv[])
{
	int soc = createSocket(1, AF_INET, 30000, inet_addr("127.0.0.1"), 5);
	cli = accept(soc, NULL, NULL);
	if (cli < 0)
		exit_on_error("accept");
	{ 
		if (fcntl(cli, F_SETOWN, getpid()) < 0)
			exit_on_error("fcntl:F_SETOWN");

		// handle SIGURG
		struct sigaction act;
		act.sa_handler = urgent_data_handler;
		act.sa_flags = SA_RESTART;  // since this flag is set, functions like read and write will continue
									// working after they are interrupted instead of returning an EINTR error
		int rt = sigaction(SIGURG, &act, NULL);
		if (rt != 0)
			exit_on_error("sigaction");
	}

	while(1) {
		char buf[BUFSIZ];
		int rt;
		rt = read(cli, buf, BUFSIZ);  // ordinary read events do not read out-of-band data
		if (rt == 0)
			break;
		strcat(buf, " :ordinary\n");
		write(STDOUT_FILENO, buf, rt + 11);
	}
	fflush(stdout);
	close(cli);

	return 0;
}
