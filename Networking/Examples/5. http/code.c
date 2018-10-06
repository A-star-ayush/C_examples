#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netdb.h>  // for hostent & herror
#include <string.h>

int main(int argc, char const *argv[])
{	
	char* hostname = "wps.pearsoned.ca";
	char ip[100];
	struct hostent* he = gethostbyname(hostname);  // **use of gethostbyname is deprecated, use getaddrinfo instead**
	if( he == NULL){
		herror("gethostbyname");  // note: not perror but herror which reflects the status of h_errno
		exit(EXIT_FAILURE);
	}

	struct in_addr** addrList = (struct in_addr**)he->h_addr_list;
	strcpy(ip, inet_ntoa(*addrList[0]));

	printf("%s resolved to %s\n", hostname, ip); 

	int soc = socket(PF_INET, SOCK_STREAM, 0);
	struct sockaddr_in gserver = { AF_INET, htons(80), inet_addr(ip) };
			// port 80 is the one that listens to or expects to receive from a web client
	int rt = connect(soc, (struct sockaddr*)&gserver, sizeof(struct sockaddr_in));
	if(rt==-1){
		perror("connect");
		exit(EXIT_FAILURE);
	}

	if(send(soc, "GET / HTTP/1.1\r\n\r\n", 23, 0)<0){
		perror("send");
		exit(EXIT_FAILURE);
	}

	char rply[20000];
	if(recv(soc, rply, 20000, 0) < 0){
		perror("recv");
		exit(EXIT_FAILURE);
	}

	puts(rply);
	
	close(soc);
	return 0;
}

