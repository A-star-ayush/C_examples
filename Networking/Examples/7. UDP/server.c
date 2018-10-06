#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <time.h>

#define exit_on_error(msg) \
	do { perror(msg); exit(EXIT_FAILURE); } while(0)

#define scast(x) (struct sockaddr*)x

int main(int argc, char const *argv[])
{
	int fd = socket(PF_INET, SOCK_DGRAM, 0);
	if(fd==-1) exit_on_error("socket");

	int sz = sizeof(struct sockaddr_in);
	struct sockaddr_in lAdd = { .sin_family = AF_INET, .sin_port = 0, .sin_addr = htonl(INADDR_ANY) };  // 0 for a random port
	memset(&lAdd.sin_zero, 0, 8);		// this random port is not reflected in the port field since that is constant
											// to see the new port use getsockname
	if(bind(fd, scast(&lAdd), sz)==-1) exit_on_error("bind");
	if(getsockname(fd, scast(&lAdd), &sz)==-1) exit_on_error("getsockname");

	printf("Port number assigned: %d\n", ntohs(lAdd.sin_port));  // ** very imp to use ntohs otherwise if you specify this port
															// on an Little Endian client, the connection will be refused
	// no listen operation on udp protocol						// or something
	// no accept operation on udp protocol

	struct sockaddr_in cAdd;
	
	char buf[BUFSIZ] = { 0 };
	recvfrom(fd, buf, BUFSIZ, 0, scast(&cAdd), &sz);
	printf("Received: %s\n", buf);
	time_t t = time(NULL);
	ssize_t sent = sendto(fd, &t, sizeof t, 0, scast(&cAdd), sz);
	puts("Sent time");
	if(sent==-1) exit_on_error("sendto");

	close(fd);

	return 0;
}
