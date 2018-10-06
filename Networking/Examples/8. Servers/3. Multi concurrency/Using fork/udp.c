#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h>
#include <strings.h>
#include <sys/ioctl.h>
#include <signal.h>

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

void reaper(int sig){
	wait(NULL);
}

/* Using fork things simply work although we have declared a single buffer buf and a single cAdd structure. This is because 
   upon forking the child initially points to the mem tables of the parent only and when something (like buf) is modified 
   it forks its own copy of that variabile. */
   
int main(int argc, char const *argv[])
{
	int fd, sz, rt = 0;
	fd = createSocket(0, AF_INET, 20000, INADDR_ANY, 5); // the last paramet is irrelevant for udp connections
	sz = sizeof(struct sockaddr_in);
	{ int val = 0; ioctl(fd, FIONBIO, &val); }
	char buf[BUFSIZ];
	signal(SIGCHLD, reaper);
	while(1){
		struct sockaddr_in cAdd;
		rt = recvfrom(fd, buf, BUFSIZ, 0, aCast(&cAdd), &sz);
		if(rt==-1) exit_on_error("recvfrom");

		pid_t id = fork();
		if(id == 0){
			struct sockaddr_in add;
			memcpy(&add, &cAdd, sizeof(add));
			rt = sendto(fd, buf, rt, 0, aCast(&add), sz);
			if(rt==-1) exit_on_error("sento");
			exit(0);
		}
		else if(id > 0) {}
		else exit_on_error("fork");
	}
	close(fd);
	return 0;
}