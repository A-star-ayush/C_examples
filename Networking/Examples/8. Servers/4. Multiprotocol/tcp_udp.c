#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/select.h>

#define exit_on_error(msg) \
	do { perror(msg); exit(EXIT_FAILURE); } while(0)

#define aCast(x) (struct sockaddr*)x

int createSocket(int type, int aFm, short pNum, int bAdd, int nListen){
	struct sockaddr_in lAdd;
	int fd, rt, sz = sizeof(struct sockaddr_in);
	{
		int M, socType;
		M = -(type==1);
		socType = (SOCK_STREAM & M) | (SOCK_DGRAM & ~M);
		fd = socket(aFm, socType, 0);
	}
	if(fd==-1) exit_on_error("createSocket:socket");
	bzero(&lAdd, sz);
	lAdd.sin_family = aFm;
	lAdd.sin_port = htons(pNum);
	lAdd.sin_addr.s_addr = bAdd;
	rt = bind(fd, aCast(&lAdd), sz);
	if(rt==-1) exit_on_error("createSocket:bind");
	if(type==1) { rt = listen(fd, nListen);  // listen operation not supported for UDP
				  if(rt==-1) exit_on_error("createSocket:listen"); }
	return fd;
}

void serveUDPclient(int fd){
	char buff[BUFSIZ];
	int rt;
	puts("Serving a particular udp client");
	int sz = sizeof(struct sockaddr_in);
	struct sockaddr_in cadd;
	rt = recvfrom(fd, buff, BUFSIZ, 0, aCast(&cadd), &sz);
	if(rt < 0) exit_on_error("serveUDPclient:recvfrom");
	rt = sendto(fd, buff, rt, 0, aCast(&cadd), sz);
	if(rt < 0) exit_on_error("serveUDPclient:sendto");
}

int serveTCPclient(int fd){
	char buf[BUFSIZ];
	int rt;
	printf("Serving client with fd: %d\n", fd);
	rt = read(fd, buf, BUFSIZ);
	if(rt < 0) exit_on_error("serveTCPclient:read");
	rt = write(fd, buf, rt);						
	if(rt < 0) exit_on_error("serveTCPclient:read");

	return 0;
}

/* A single server accepting both TCP and UDP requests */

int main(int argc, char const *argv[])
{
	int sz = sizeof(struct sockaddr_in), nfds = getdtablesize(), fd;
	int tsoc = createSocket(1, AF_INET, 20000, INADDR_ANY, 5); // tcp socket
	int usoc = createSocket(0, AF_INET, 20000, INADDR_ANY, 0); // udp socket
	// Note: same port number was used for both tcp and udp
	fd_set rfds, afds;
	FD_ZERO(&afds);
	FD_SET(tsoc, &afds);
	FD_SET(usoc, &afds);

	while(1){
		memcpy(&rfds, &afds, sizeof(rfds));
		int rt = select(nfds, &rfds, NULL, NULL, NULL);
		if(rt < 0) exit_on_error("select");

		if(FD_ISSET(tsoc, &rfds)){
			struct sockaddr_in cadd;
			int cli = accept(tsoc, aCast(&cadd), &sz);
			if(cli < 0) exit_on_error("accept");
			FD_SET(cli, &afds);
		}

		for(fd=0;fd<nfds;++fd){
			if(fd==usoc && FD_ISSET(usoc, &rfds))
				serveUDPclient(fd);  // we do not close fd for udp even if we are done serving the client, otherwise you won't
			else if(fd!=tsoc && FD_ISSET(fd, &rfds))	// be able to accept any future clients (sincs udp has a single fd)
				if(!serveTCPclient(fd)){
					close(fd);
					FD_CLR(fd, &afds);
					printf("Closed connection with %d\n", fd);
				}
		}
	}

	return 0;
}