#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/select.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <net/if.h>

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

int main(int argc, char const *argv[])
{
	int lsoc = createSocket(0, AF_INET, 20000, INADDR_ANY, 0);

	int rt = 0;
	char buf[BUFSIZ];
	struct sockaddr_in padd;
	int sz = sizeof(struct sockaddr_in);

	int broadcast = 1;
 	rt = setsockopt(lsoc, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast));
 	if(rt < 0) exit_on_error("setsockopt");

	do{
		rt = recvfrom(lsoc, buf, BUFSIZ, 0, aCast(&padd), &sz);
		printf("%s\n", buf);
	}while(rt<=0);

	return 0;
}