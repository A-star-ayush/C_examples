#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

#define error_handler(msg) \
	do { perror(msg); exit(EXIT_FAILURE); } while(0)

/* The program uses a pipe to communicate between a parent and its child process. Parent writes whatever was passed to it 
	in argv[1], The child reads the same and displays it on the standard output. */
/* The child inherit the pipes from the parent. Before using them, both the parent and the child closes the end of the
   pipe they do not need since pipes are unidirectional. */

int main(int argc, char const *argv[])
{	
	int pipefd[2];
	pid_t cpid;
	char bf;  // to read/wrtie a single character

	if(argc!=2){
		fprintf(stderr, "Usuage: %s <string>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	if(pipe(pipefd)) error_handler("pipe");

	cpid = fork();

	if(cpid==-1) error_handler("fork");

	if(cpid==0){  // Inside the child
		close(pipefd[1]);  // closes the writing end beacuse the child is going to read the data.
		while(read(pipefd[0], &bf, 1)>0) write(STDOUT_FILENO, &bf, 1);
		write(STDOUT_FILENO, "\n", 1);
		close(pipefd[0]);  // closes the reading end as well since it is done reading
		exit(EXIT_SUCCESS);
	}

	else{  // Inside the parent
		close(pipefd[0]); // similarly the parent closes the reading end
		write(pipefd[1], argv[1], strlen(argv[1]));
		close(pipefd[1]);  // done with writing [IMPORTANT] otherwise read will remain blocked; waiting for more to be written
		wait(NULL);				// we can programme for non-blocking by setting O_NONBLOCK using fcntl or describing it as a
		exit(EXIT_SUCCESS);			// flag in pipe2
	}


	return 0;
}