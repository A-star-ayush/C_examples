#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

#define exit_on_error(msg) \
	do { perror(msg); exit(EXIT_FAILURE); } while(0)

int main(int argc, char const *argv[])
{
	int sfd = socket(PF_INET, SOCK_STREAM, 0);
	if(sfd==-1) exit_on_error("socket");
	
	struct sockaddr_in saddr = {AF_INET, htons(20000), inet_addr("127.0.0.1")};  // connecting to the local host
	if(connect(sfd, (struct sockaddr*)&saddr, sizeof(struct sockaddr))==-1) exit_on_error("connect");

	char msg[BUFSIZ];
	if(read(sfd, msg, BUFSIZ)==-1) exit_on_error("read");
	printf("%s\n",msg);
	if(write(sfd, "Hello from the other side", 26)==-1) exit_on_error("write");

	close(sfd);
	
	return 0;
}