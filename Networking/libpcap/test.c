#include <stdio.h>
#include <pcap.h>
#include <stdlib.h>
#include <arpa/inet.h>

// compile with -lpcap
// Normal users generally don't have the permission to capture on interfaces other than the bluetooth interface. Run the
// program with sudo to permit capture on other interfaces as well

int main(int argc, char *argv[]){

	char errbuf[PCAP_ERRBUF_SIZE];

	/*pcap_if_t* devices;
	int rt = pcap_findalldevs(&devices, errbuf);
	if(rt < 0) {
		fprintf(stderr, "%s\n", errbuf);
		exit(1);
	}

	if(devices == NULL)  // no devices found is not considered an error so rt will be 0 for this case
		printf("No devices were found\n");
	else {
		while(devices!=NULL){
			printf("\nName: %s\n", devices->name);
			while(devices->addresses!=NULL){
				struct sockaddr* add = devices->addresses->addr;
				switch(add->sa_family){
					char addBuf[INET6_ADDRSTRLEN];
					case AF_INET:
						printf("Address: %s\n", inet_ntop(AF_INET, &((struct sockaddr_in*)add)->sin_addr, addBuf,
															INET_ADDRSTRLEN));
						break;
					case AF_INET6:
						printf("Address: %s\n", inet_ntop(AF_INET6, &((struct sockaddr_in6*)add)->sin6_addr,addBuf, 
															INET6_ADDRSTRLEN));
						break;
					default:
						puts("Address: Not an IPv4 or IPv6 address");

				}
				devices->addresses = devices->addresses->next;
			}
			devices = devices->next;
		}
	}*/

	return(0);
}