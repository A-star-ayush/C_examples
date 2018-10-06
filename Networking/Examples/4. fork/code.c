#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>

// error checking skipped for brevity

#define PRT_NUM 20000

int main(int argc, char const *argv[])
{
	pid_t p = fork();
	if(p==-1){
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if(p>0){  // Inside parent
		int hSoc = socket(PF_INET, SOCK_STREAM, 0);
		struct sockaddr_in sadd = { AF_INET, htons(PRT_NUM), htonl(INADDR_ANY) };
		bind(hSoc, (struct sockaddr*)&sadd, sizeof(struct sockaddr));
		listen(hSoc, 1);
		struct sockaddr_in cadd = {0};
		int sz = sizeof(struct sockaddr);
		int cfd = accept(hSoc, (struct sockaddr*)&cadd, &sz);
		write(cfd, "Hello", 6);
		close(cfd);
		wait(NULL);
		close(hSoc);
		exit(EXIT_SUCCESS);
		
	}
	else{ // Inside child
		int server = socket(PF_INET, SOCK_STREAM, 0);
		struct sockaddr_in sadd = { AF_INET, htons(PRT_NUM), inet_addr("127.0.0.1") };
		connect(server, (struct sockaddr*)&sadd, sizeof(struct sockaddr));
		char msg[BUFSIZ];
		read(server, msg, BUFSIZ);
		printf("Message from server: %s\n", msg);
		close(server);
		exit(EXIT_SUCCESS);
	}	
	return 0;
}