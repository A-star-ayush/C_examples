#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <arpa/inet.h>

// port numbers less than 1024 are considered priveleged ports so an unprivileged process cannot bind to them
// In this example we assume that the setuid bit is set for the executable by the root (ls -l shows a 's'
// in its description in the place of 'x'). Thus even if a normal user executes this file, the executable will be able to 
// bind to one of the privileged ports. Try removing the setuid bit and you will notice that the bind function fails.

#define error_handler(msg) \
	do { perror(msg); exit(EXIT_FAILURE); } while(0)

static uid_t euid, ruid;  // effective and real ids

void undo_setuid(){
	int status;
	#ifdef _POSIX_SAVED_IDS  // not all systems have the setuid function so we provide a fallback 
	status = seteuid(ruid);
	#else 
	status = setreuid(euid, ruid);
	#endif
	if(status<0) error_handler("undo_setuid");
}

void do_setuid(){
	int status;
	#ifdef _POSIX_SAVED_IDS  // not all systems have the setuid function so we provide a backup
	status = seteuid(euid);
	#else 
	status = setreuid(ruid, euid);
	#endif
	if(status<0) error_handler("undo_setuid");
}

int main(int argc, char const *argv[])
{
	ruid = getuid();
	euid = geteuid();
	printf("Original-> real id: %d effective id: %d\n", getuid(), geteuid());

	// *** we only need to raise the privileges during binding, so start by dropping our privileges first. This is important
	// from a security point off view ***

	undo_setuid();
	
	printf("Before socket-> real id: %d effective id: %d\n", getuid(), geteuid());

	int soc = socket(PF_INET, SOCK_STREAM, 0);
	if(soc==-1) error_handler("socket");

	struct sockaddr_in iadd = { AF_INET, htons(70), htonl(INADDR_ANY) };

	// now gaining root privileges to bind the socket to a privileged port
	do_setuid();

	printf("Before bind-> real id: %d effective id: %d\n", getuid(), geteuid());

	int rt = bind(soc, (struct sockaddr*)&iadd, sizeof(iadd));
	if(rt==-1) error_handler("bind");

	rt = listen(soc, 5);
	if(rt==-1) error_handler("bind");

	puts("passive server made");
	close(soc);
	puts("passive server closed");
	// Note that the code just checks if the bind function works and is not meant to really work as a server (although not much
	// is left to implement to make it active)

	undo_setuid();  // again dropping privileges
	printf("Before exiting-> real id: %d effective id: %d\n", getuid(), geteuid());

	return 0;
}