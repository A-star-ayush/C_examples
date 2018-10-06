#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>

// This program is run when an incoming connection is accepted as per the configuration specified under
// under /etc/xinetd.d/ [my file: /etc/xinetd.d/myServ and /etc/xinetd.d/hello] 

// Note: client program's writes to the socket are read as stdin to the server program, 
// and stdout from the server program are read from the client's socket reads but dont use the scanf or printf functions
// to handle them

int main(int argc, char* argv[])
{
	char buffer[BUFSIZ];
	
	uid_t id = getuid();
	uid_t eid = geteuid();

	sprintf(buffer, "id: %d eid: %d\n",id,eid);
	write(STDOUT_FILENO, buffer, strlen(buffer)+1);

	while(1){
		int rt = read(STDIN_FILENO, buffer, BUFSIZ);
		if(rt < 0) perror("read");
		write(STDOUT_FILENO, buffer, rt);
	}	
	return 0;
}

/* Note: Manual logging or creating files in cwd might not work. One can use the inbuilt logging features in xinetd as well.
   Reason: xinetd executes the server program with the current working directory (cwd) set as / (the root directory).
   		   Normal user don't have the rights to create or edit files on / directly.
   Workarounds: Specify the absolute path where the executing user has to rights to create files.
   				OR Run the server program as root. But that can raise security concerns. */