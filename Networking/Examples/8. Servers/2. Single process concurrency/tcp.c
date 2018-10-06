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
 
int serveClient(int fd){
	char buf[BUFSIZ];
	int rt;

	printf("Serving client with fd: %d\n", fd);

	rt = read(fd, buf, BUFSIZ);  // we expect it to proceed without blocking since 'select' informed us that data was
	if(rt < 0) exit_on_error("serveClient:read");  // available to be read from this fd. But there can be problems with using
													// select. A separate text file addresses the issue in the same folder.
	rt = write(fd, buf, rt);						
	if(rt < 0) exit_on_error("serveClient:read");

	return 0;  // we are doing a single request response here 
}

int main(int argc, char const *argv[])
{	
	int sz = sizeof(struct sockaddr_in), fd;

	int lsoc = createSocket(1, AF_INET, 20000, INADDR_ANY, 5);

	int nfds = getdtablesize();  // returns the maximum number of files a process can have open [max fd no + 1]
	fd_set rfds, afds;					// 1024 when last tested ... if we know we are not dealing with that king of workload
	FD_ZERO(&afds);									// it would be good to reduce this number to a reasonable one.
	FD_SET(lsoc, &afds);
			/*** we maintain two sets: rfds and afds mainly because select() overwrites the fd_set variables whose pointers 
			   are passed in as arguments 2-4, telling it what to wait for. This makes a typical loop having to either  
			   have a backup copy of the variables, or even worse, do the loop to populate the bitmasks every time select() 
			   is to be called. [poll doesn't suffer from this] ***/
			   
// We need not maintian two sets if all we are doing is a single request response thing (like in this example). In that case
// just bring the FD_SET(lsoc, &afds) inside the while(1) loop so that it is reset everytime the loop is run. Example: day
// time multi protocol client - checkout Guides/C/Network Programming/tenouk.com/7...

	while(1){
		memcpy(&rfds, &afds, sizeof(rfds));
		int rt = select(nfds, &rfds, NULL, NULL, NULL);
		if(rt < 0) exit_on_error("select");
		if(FD_ISSET(lsoc, &rfds)){  // we have a new connection request

			struct sockadd_in cAdd;
			int fsoc = accept(lsoc, aCast(&cAdd), &sz);
			if(fsoc < 0) exit_on_error("accept");
			FD_SET(fsoc, &afds);
			int val = 0;
			ioctl(fsoc, FIONBIO, &val); // enabling blocking (just to test whether select is working as desired)
		}									// IO calls wouldn't block if select was working as desired
	
		for(fd=0;fd<nfds;++fd){
			if(fd!=lsoc && FD_ISSET(fd, &rfds))  // to eliminate the first check unset lsoc if found set in the previous
																	// conditional
				if(serveClient(fd)==0){  // done serving this client
					close(fd);
					FD_CLR(fd, &afds);
					printf("Closed connection with no %d\n", fd);
				}
		}
	}

	
	return 0;
}

