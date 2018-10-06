#include <stdio.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/select.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <net/if.h>  // for IFF_BROADCAST

// error checking skipped for brevity

// INET6_ADDRSTRLEN - constant that represents the maximum size of a valid ipv6 address "string" - 46 when last checked
// INET_ADDRSTRLEN - constant that represents the maximum size of a valid ipv4 address "string" - 16 when last checked
// Use struct sockaddr_storage if you want to deal adequately with both ipv4 and ipv6 sockets.

int main(int argc, char const *argv[])
{
		
	/*struct ifaddrs* intr;          
	int rt = getifaddrs(&intr);  // creates a linked list of structures describing the local network interfaces
	while(intr){
		puts("###########################");
		
		printf("Name: %s\n", intr->ifa_name);
		if(intr->ifa_addr && intr->ifa_addr->sa_family == AF_INET){
			
			struct sockaddr_in* x = (struct sockaddr_in*)intr->ifa_addr;
			printf("IP: %s\n", inet_ntoa(x->sin_addr));
				
			if(intr->ifa_netmask){
				struct sockaddr_in* x = (struct sockaddr_in*)intr->ifa_netmask;
				printf("Netmask: %s\n", inet_ntoa(x->sin_addr));
			}
			else puts("No netmask available");
		
			if(intr->ifa_flags & IFF_BROADCAST){
				struct sockaddr_in* x = (struct sockaddr_in*)intr->ifa_broadaddr;
				printf("Broadcast: %s\n", inet_ntoa(x->sin_addr));
			}
			else puts("No broadcast available");
		}
		else puts("Either address information not available or address family doesn't match AF_INET");
		
		intr = intr->ifa_next;
	}

	freeifaddrs(intr);  // remember to free since data returned by getifaddrs is dynamically returned
	
	// another way of getting this information is using ioctl (checkout man 7 netdevice)
 	// webpage saved under Saved\ Webpages
	*/



	/*struct in_addr ex;
	inet_aton("172.20.2.213", &ex);
	printf("%u\n", ex.s_addr);
	printf("%s\n", inet_ntoa(ex));
	struct in_addr net = {  inet_netof(ex) };  // extracts the internet network part of the address
	struct in_addr lna = {  inet_lnaof(ex) };  // extracts the local address 
	printf("%s\n", inet_ntoa(net));
	printf("%s\n", inet_ntoa(lna));
	struct in_addr ex2 = inet_makeaddr(net.s_addr, lna.s_addr);
	printf("%s\n", inet_ntoa(ex2));
	*/
	
	return 0;
}

/************* Refer to man 7 socket for know more about socket options **************************/

/* int flag = 1;
   int result = setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (char *) &flag, sizeof(int));
*/                                         // setting TCP_NODELAY to negate Nagle's algorithm & allow small buffer write
   											// checkout TCP_CORK as well

/*
 int broadcast = 1; // try char broadcast = '1' if it doesn't work
 setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof broadcast);  // asking kernel the permission to broadcast
  // 1) 255.255.255.255 - limited broadcast 2) 127.255.255.255 - local broadcast 3) network | ~subnet - subnetted broadcast
*/

/* 
   int val = 0;  // 1 for non-blocking I/O
   ioctl(cfd, FIONBIO, (char*)&val);  // enabling BLOCKING mode I/O on cfd .. <sys/ioctl.h>
*/

/* It is recommended to use inet_aton instead of inet_addr beacuse the latter returns -1 in case of error but -1 is 
    a valid address: 255.255.255.255. One can prefer using inet_pton which deals with both ipv4 and ipv6 but does not
    accept shorthand notations. getaddrinfo is yet another option. */

/*
 int state = 1;
 setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &state, sizeof(int));
		// to reuse the address without waiting (skipping TIMED_WAIT state) .. must be done before binding
*/

/*
    struct timeval timeout;      
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;

    if (setsockopt (sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout,
                sizeof(timeout)) < 0)
        error("setsockopt failed\n");

    if (setsockopt (sockfd, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout,
                sizeof(timeout)) < 0)
        error("setsockopt failed\n");
*/


/*
	int buffersize = 64*1024;  // 64k
	setsockopt(socket, SOL_SOCKET, SO_SNDBUF, (char *) &buffersize, sizeof(buffersize));
					// similarly we also have the SO_RCVBUF
*/

/* ioctl(fd,FIONREAD,&bytes_available);    // to read the number of bytes avaible in the receive buffer */
