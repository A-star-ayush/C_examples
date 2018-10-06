#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h>
#include <strings.h>
#include <sys/ioctl.h>
#include <pthread.h>
#include <stdint.h>

// link with -pthread

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

void* serveClient(void* _cfd){
	static int i = 1;
	pthread_detach(pthread_self());
	int cfd = (intptr_t)_cfd, rt = 0;
	{ int val = 0; ioctl(cfd, FIONBIO, &val); }
	char buf[BUFSIZ];
	while( (rt = read(cfd, buf, BUFSIZ)) > 0){
		printf("Read from %d: %s\n", i, buf);
		rt = write(cfd, buf, rt);
		if(rt == -1) exit_on_error("write");
		printf("Wrote to %d: %s\n", i, buf);
	}
	close(cfd);
	printf("Closed Connection #%d\n", i++);
	return NULL;
}

int main(int argc, char const *argv[])
{
	int fd = createSocket(1, AF_INET, 20000, INADDR_ANY, 5);
	int sz = sizeof(struct sockaddr_in);

	while(1){
		struct sockaddr_in cAdd;
		int cfd = accept(fd, aCast(&cAdd), &sz);
		if(cfd==-1) exit_on_error("accept");
		
		pthread_t pt;
		pthread_create(&pt, NULL, serveClient, (void*)(intptr_t)cfd);
	}
	close(fd);
	return 0;
}