#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>

// refer to Docs/Executing\ a\ file\Daemons for the theory
// A much simpler way is to use the daemon function directly

// To compile: gcc -o myDaemon daemon.c 
// To run: ./myDaemon 
// To verify if a daemon was created: ps -xj | grep myDaemon
// Also locate the log file (usually /var/log/syslog) and check whether our messages made into it or not

#define eoe(msg) \
	do { perror(msg); exit(EXIT_FAILURE); } while(0)

static void skeleton_daemon(){
	pid_t pid = fork();
	if(pid < 0) eoe("fork");
	if(pid > 0) exit(EXIT_SUCCESS);  // #1. Fork and terminate the parent

	pid = setsid();  // #2. create a new session
	if(pid < 0) eoe("setsid");

	signal(SIGCHLD, SIG_IGN);  // #3. Catch or ignore the signals as per the requirement
    signal(SIGHUP, SIG_IGN);

	pid = fork();
	if(pid < 0) eoe("fork");
	if(pid > 0) exit(EXIT_SUCCESS);  // #4. Fork again and terminate the parent

	int rt = chdir("/");
	if(rt < 0) eoe("chdir");   // #5. change the directory of the daemon

	umask(0);  // #6. Optionally change the umask (this command always succeeds)

	int x;
    for (x = sysconf(_SC_OPEN_MAX); x>=0; x--) close (x);  // #7. Close all open file descriptors

    openlog ("myDaemon", LOG_PID, LOG_DAEMON);
}

int main(int argc, char const *argv[]){
	skeleton_daemon();
	while(1){
		// TODO: Insert daemon code here
		syslog(LOG_NOTICE, "myDaemon started");
		sleep(20);
		break;
	}

	syslog(LOG_NOTICE, "myDaemon terminated");
	closelog();

	return EXIT_SUCCESS;
}