#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

/*Prints the host name and domain name*/

#define exit_on_error(msg) \
	do {perror(msg); exit(EXIT_FAILURE);} while(0)

int main(int argc, char const *argv[])
{
	long n = sysconf(_SC_HOST_NAME_MAX);  // to obtain the maximum lenght of the hostname
	if(n==-1)
		exit_on_error("sysconf");

	char hostName[n];
	char domainName[n];

	if(gethostname(hostName, n)==-1)
		exit_on_error("gethostname");

	printf("Host name: %s\n", hostName);

	if(getdomainname(domainName, n)==-1)
		exit_on_error("getdomainname");

	printf("Domain Name: %s\n",domainName);

	return 0;
}
