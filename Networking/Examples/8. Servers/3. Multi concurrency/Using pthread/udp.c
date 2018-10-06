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

int fd;

struct TCB {
	struct sockaddr_in* cAdd;
	char* buf; // buffer associated with the client
	int rd;  // no of bytes read
};

void* serveClient(void* _arg){
	static int i = 1;
	pthread_detach(pthread_self());
	
	struct TCB* arg = _arg;
	struct sockaddr_in* cAdd = arg->cAdd;
	int rt = arg->rd;
	char* buf = arg->buf;
	
	int sz = sizeof(struct sockaddr_in);
	rt = sendto(fd, buf, rt, 0, aCast(cAdd), sz);
	if(rt==-1) exit_on_error("sento");
	printf("Done for Connection #%d\n", i++);
	
	free(cAdd);
	free(buf);
	free(arg);  // remember to free the dynamic allocations

	return NULL;
}

int main(int argc, char const *argv[])
{
	int sz, rt = 0;
	fd = createSocket(0, AF_INET, 20000, INADDR_ANY, 5);
	sz = sizeof(struct sockaddr_in);
	{ int val = 0; ioctl(fd, FIONBIO, &val); }
	
	while(1){
		struct sockaddr_in* cAdd = malloc(sz);
		char* buf = malloc(BUFSIZ);
		struct TCB* t = malloc(sizeof(struct TCB));
		rt = recvfrom(fd, buf, BUFSIZ, 0, aCast(cAdd), &sz);
		if(rt==-1) exit_on_error("recvfrom");
		t->cAdd = cAdd;
		t->rd = rt;
		t->buf = buf;
		pthread_t pt;
		pthread_create(&pt, NULL, serveClient, t);
	}
	close(fd);
	return 0;
}