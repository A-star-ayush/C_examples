#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

// error checking skipped for brevity

int main(int argc, char const *argv[])
{
	int server = socket(PF_INET, SOCK_STREAM, 0);
	struct sockaddr_in sadd = {AF_INET, htons(20000), inet_addr("127.0.0.1")};
	connect(server, (struct sockaddr*)&sadd, sizeof(struct sockaddr));
	char msg[BUFSIZ];
	while(read(server, msg, BUFSIZ)>0)
		printf("Message received from server: %s\n", msg);
	close(server);
	return 0;
}