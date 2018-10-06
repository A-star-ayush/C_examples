#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <time.h>

#define exit_on_error(msg) \
	do { perror(msg); exit(EXIT_FAILURE); } while(0)

int main(int argc, char const *argv[])
{	
	/* Creating a socket*/
	int sfd = socket(PF_INET, SOCK_STREAM, 0);
	if(sfd==-1) exit_on_error("socket");
	
	/*Naming the socket*/
	struct sockaddr_in saddr = {AF_INET, htons(20000), htonl(INADDR_LOOPBACK)};  // specifying a port number of 0
	if(bind(sfd, (struct sockaddr*)&saddr, sizeof(struct sockaddr))==-1) exit_on_error("bind");  // binds to an available port

	/*Definning the listening capacity */
	if(listen(sfd, 5)==-1) exit_on_error("listen"); // 2nd arg is maximum length of pending connection
	// listen marks the socket as passive ,i.e, ready to accept incoming connections

	while(1){
		struct sockaddr_in caddr = {0};
		int socklen = sizeof(struct sockaddr), cfd;
		if((cfd=accept(sfd, (struct sockaddr*)&caddr, &socklen))==-1) exit_on_error("accept");
		// blocks until connection from any client
		// accept make the socket active. The client side address deatils is obtained in the struct passed as the 2nd arg.
		
		/*Moving data between server and client*/
		
		if(write(cfd, "Hello from the local host", 26)==-1) exit_on_error("write");  // mark cfd and not sfd here
		char msg[BUFSIZ];
		if(read(cfd, msg, BUFSIZ)==-1) exit_on_error("read");
		printf("%s\n", msg);
		printf("Closing the connection to %s\n",inet_ntoa(caddr.sin_addr));  // ip address to string
		close(cfd);  // once the work is done, close the accpeted connection to the client side
	}

	/*Note: since we have programmed a single threaded application, multiple clients cannot be served simaltaneously */
	
	close(sfd);  // Remember to close the open socket

	return 0;
}

/* On reusing the same port number in quick succession, socket might fail with "Address already in use". 
	That is due to the TIME_WAIT state of a TCP connection. Either you can wait for sometime or you can
	change your code to include:

    int yes = 1;
    if(setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1){
      perror("setsockopt() error");
      exit(1);
    }
    else
      printf("setsockopt() is OK.\n");
      
*/
