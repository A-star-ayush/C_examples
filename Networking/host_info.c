#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>

// gethostbyname and gethostbyaddr are now obsolete

// There are more functions to get information: 1) getaddrinfo_a 2) getnameinfo 3) getpeername 4) getsockname 

// Example of getsockname: 7. UDP/server.c. It highlights the importance of converting between host and network data orientation to make
// sense of the values pointed by the address returend by getsockname.

int main(int argc, char const *argv[])
{	
	if(argc<2){
		printf("Usage: %s <hostname> <service>\n", argv[0]); exit(1);   // ex: ./a.out goolge.com http
	}								// service is basically a port number .. checkout /etc/services for a list
										// In our case, service sets the port in each returned address structure.
	struct addrinfo hints, *x;
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;  /* Allow IPv4 or IPv6*/
	hints.ai_socktype = SOCK_DGRAM;  // if you set it to 0, you will get reponse for SOCK_STREAM, SOCK_DGRAM, SOCK_RAW
	hints.ai_flags = AI_CANONNAME;	 // in that case a single ip address might have 3 entries in the list that is returned
	hints.ai_protocol = 0; /* Any protocol*/

	int rt = getaddrinfo(argv[1], argv[2]==NULL ? NULL: argv[2], &hints, &x);
	if(rt!=0){				// either of the first or second arg can be null but not both
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rt));
		exit(EXIT_FAILURE);
	}

	char buf[INET6_ADDRSTRLEN];

	for(;x!=NULL;x=x->ai_next){
		
		if(x->ai_family == AF_INET){ 
			struct sockaddr_in* y = (struct sockaddr_in*)x->ai_addr;
			printf("Address: %s\n", inet_ntop(AF_INET, &y->sin_addr, buf, sizeof(buf)));
			printf("Port: %d\n", ntohs(y->sin_port));
		}
		else if(x->ai_family == AF_INET6){
			struct sockaddr_in6* y = (struct sockaddr_in6*)x->ai_addr;
			printf("Address: %s\n", inet_ntop(AF_INET6, &y->sin6_addr, buf, sizeof(buf))); 
			printf("Port: %d\n", ntohs(y->sin6_port));
		}
		
		printf("Canonical Name: %s\n", x->ai_canonname);
	}

	freeaddrinfo(x);  // good practise since getaddrinfo returns a list of addresses

	return 0;
}

/* // On client side: successfully connect to one of the addresses
 for (rp = result; rp != NULL; rp = rp->ai_next) {
               sfd = socket(rp->ai_family, rp->ai_socktype,
                            rp->ai_protocol);
               if (sfd == -1)
                   continue;

               if (connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1)
                   break;                  // Success 

               close(sfd);
           }
*/

/*
	The same thing can be used on the server side to bind to a address.
	A call to getaddrinfo on the server side could look like:
		getaddrinfo(NULL, argv[1], &hints, &result)
	with hints as:
		memset(&hints, 0, sizeof(struct addrinfo));
		hints.ai_family = AF_UNSPEC;    // Allow IPv4 or IPv6
        hints.ai_socktype = SOCK_DGRAM; // Datagram socket
        hints.ai_flags = AI_PASSIVE;    // For wildcard IP address .. INADDR_ANY for IPv4, IN6ADDR_ANY_INIT for IPv6
        hints.ai_protocol = 0;          // Any protocol
        hints.ai_canonname = NULL;
        hints.ai_addr = NULL;
        hints.ai_next = NULL;

*/  // AI_PASSIVE flag is what makes it suitable for server side binding
