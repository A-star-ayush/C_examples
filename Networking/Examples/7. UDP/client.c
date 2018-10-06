#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <time.h>

#define exit_on_error(msg) \
	do { perror(msg); exit(EXIT_FAILURE); } while(0)
#define nexit_on_error(r, msg) \
	do { errno = r; perror(msg); exit(EXIT_FAILURE); } while(0)

#define scast(x) (struct sockaddr*)x

int main(int argc, char const *argv[])
{
	if(argc<3){
		fprintf(stderr, "%s <host address> <port>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	int port = atoi(argv[2]);

	int fd = socket(PF_INET, SOCK_DGRAM, 0);
	struct sockaddr_in fAdd = { .sin_family = AF_INET, .sin_port = htons(port) };
	memset(fAdd.sin_zero, 0, 8);
	int rt = inet_aton(argv[1], &fAdd.sin_addr);
	if(rt==0){
		fprintf(stderr, "Invalid address\n");
		exit(EXIT_FAILURE);
	}

	time_t t;
	int sz = sizeof(struct sockaddr_in);
	ssize_t r;
	if(connect(fd, scast(&fAdd), sz)==-1) exit_on_error("connect");
	puts("YES");
	r = send(fd, "RUN", 4, 0);
	if(r==-1) exit_on_error("send");
	r = recv(fd, &t, sizeof t, 0);
	if(r==-1) exit_on_error("recv");
	printf("Time: %s", ctime(&t));

	return 0;
}

/* In case of UDP protocol, if you use connect no handshake will be performed but it will enable you to use recv and
	send directly without specifying the address each time */
