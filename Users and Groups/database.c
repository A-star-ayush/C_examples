#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>  // for getpwuid
#include <utmpx.h>
#include <utmp.h>  // for the UT_LINESIZE and UT_NAMESIZE constants
#include <string.h>
#include <time.h>
#include <unistd.h>

// run with sudo

int main(int argc, char const *argv[])
{
	puts("\nBefore adding a new entry\n");
	system("who");  // displays who is logged in

	/* Putting a new entry in the user database */
	
	// using the 'x' versions for greater portability
	struct utmpx entry = { .ut_type=USER_PROCESS, .ut_pid=getpid() };
	strcpy(entry.ut_line, ttyname(STDIN_FILENO) + strlen("/dev/"));  
	// ttyname returns name of the terminal associated with the passed file descriptor
	// strlen then advances the pointer to the desired location  /* only correct for ptys named /dev/tty[pqr][0-9a-z] */
	strcpy(entry.ut_id, ttyname(STDIN_FILENO) + strlen("/dev/tty"));
	time(&entry.ut_time);
	strcpy(entry.ut_user, getpwuid(getuid())->pw_name);  // inquiring the /etc/passwd file for the user name
	memset(entry.ut_host, 0, UT_HOSTSIZE);
	entry.ut_addr = 0;
	setutxent();
	pututxline(&entry);

	puts("\nAfter adding a new entry\n");
	system("who");

	/* Deleting the entry */

	entry.ut_type = DEAD_PROCESS;
	memset(entry.ut_line, 0, UT_LINESIZE);
	entry.ut_time = 0;
	memset(entry.ut_user, 0, UT_NAMESIZE);
	setutxent();
	pututxline(&entry);

	puts("\nAfter removing the new entry\n");
	system("who");

	endutxent();

	return 0;
}