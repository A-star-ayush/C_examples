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
#include <time.h>
#include <assert.h>

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
	if(type==1) { rt = listen(fd, nListen);
				  if(rt==-1) exit_on_error("createSocket:listen"); }
	return fd;
}

int TCPecho(int fd){
	static char buf[BUFSIZ];
	int rt;
	printf("Serving client with fd: %d\n", fd);
	rt = read(fd, buf, BUFSIZ);
	if(rt < 0) exit_on_error("serveTCPclient:read");
	rt = write(fd, buf, rt);						
	if(rt < 0) exit_on_error("serveTCPclient:read");

	return 0;
}

int TCPdayTime(int fd){
    time_t t = time(NULL);
    if((time_t)t==-1) exit_on_error("TCPdaytime:time");
    char* tstr = ctime(&t);
    if(tstr == NULL) exit_on_error("TCPdaytime:ctime");
    int rt = write(fd, tstr, strlen(tstr)+1);  // +1 to include '\0' while transmitting
    if(rt < 0) exit_on_error("TCPdayTime:write");
    return 0;
}

int UDPecho(int fd){
	static char buff[BUFSIZ];
	int rt;
	puts("Serving a particular udp client");
	int sz = sizeof(struct sockaddr_in);
	struct sockaddr_in cadd;
	rt = recvfrom(fd, buff, BUFSIZ, 0, aCast(&cadd), &sz);
	if(rt < 0) exit_on_error("serveUDPclient:recvfrom");
	rt = sendto(fd, buff, rt, 0, aCast(&cadd), sz);
	if(rt < 0) exit_on_error("serveUDPclient:sendto");
	return 0;
}

int main(int argc, char const *argv[])
{
	#define TCP 1
	#define UDP 0

	int nfds = 0, sz = sizeof(struct sockaddr_in);

	struct service {
		char* name; 
		int (*func)(int);
		int fd, proto, portNum, qLen;
	};

	typedef struct service service;

	service services[] = {
		{"echo_tcp", TCPecho, 0, TCP, 20000, 5},
		{"dayTime_tcp", TCPdayTime, 0, TCP, 20001, 5},
		{"echo_udp", UDPecho, 0, UDP, 20000, 0},
		{0,0,0,0}
	};

	int map_size = getdtablesize();
	service* fd2sv[map_size];  // fd to service mapping
	{
		int i;
		for(i=0;i<map_size;++i) fd2sv[i]=NULL;
	}
	fd_set afds, rfds;
	FD_ZERO(&afds);

	service* x;

	for(x = services; x->name; ++x){
		x->fd = createSocket(x->proto, AF_INET, x->portNum, INADDR_ANY, x->qLen);
		FD_SET(x->fd, &afds);
		fd2sv[x->fd] = x;
		nfds = nfds ^ ((x->fd^nfds) & -(x->fd>nfds));  // maximum file desciptor
	}

	nfds += 1;  // #fd = max fd + 1

	int fd, sz_rfds = sizeof(rfds);

	while(1){
		memcpy(&rfds, &afds, sz_rfds);
		int rt = select(nfds, &rfds, NULL, NULL, NULL);
		if(rt < 0) exit_on_error("select");

		for(x = services; x->name; ++x){
			if(FD_ISSET(x->fd, &rfds)){
				if(x->proto == TCP){
					int cli = accept(x->fd, NULL, NULL); // we did not catch client's address because in this simple
															// demo we are not ineterested in it
					if(cli < 0) exit_on_error("accept");
					FD_SET(cli, &afds);
					fd2sv[cli] = x;
					nfds = nfds ^ ((cli^nfds) & -(cli>nfds));
					nfds += 1;
				}											
				else{
					x->func(x->fd);
				}
				FD_CLR(x->fd, &rfds);  // we clear them here so that they do not get choosen in our next for loop
			}
		}
		
		for(fd=0;fd<nfds;++fd){
			if(FD_ISSET(fd, &rfds)){
				assert(fd2sv[fd]);  // only for debugging
				if(fd2sv[fd]->func(fd)==0){  // done serving the client
					close(fd);
					FD_CLR(fd, &afds);
					fd2sv[fd] = NULL;
					// we can modify nfds to reflect the new maximum but skipped it here (may we shouldn't)
				}
			}
		}
	}
i
	/* The 2 loops above can be combined. Checkout concurrent.c for a different implementation.
	   The same effect might not be possible with an iterative approach (due to single process driven nature), so one might 
           want to stick with the 2 for loop implementation above. */

	#undef TCP
	#undef UDP

	return 0;
}
