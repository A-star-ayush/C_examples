#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
// link with -pthread

void* reader(void *_fd){
	pthread_detach(pthread_self());
	int fd = (intptr_t)_fd;
	char rmsg[BUFSIZ];
	while(1){
		if(read(fd, rmsg, BUFSIZ)>0){
			if(strcasecmp(rmsg, "quit\n")==0) exit(0);
			printf("%s", rmsg);
		}
	}
	return NULL;
}

void* writer(void *_fd){
	pthread_detach(pthread_self());
	int fd = (intptr_t)_fd;
	size_t n = 20;
	char* wmsg = malloc(n + 1);
	while(1){
		int nbytes = getline(&wmsg, &n, stdin);
		if(nbytes>1) write(fd, wmsg, nbytes); 
	}
	return NULL;
}

int main(int argc, char const *argv[])
{
	int soc = socket(PF_INET, SOCK_STREAM, 0);
	struct sockaddr_in add = { AF_INET, htons(20000), htonl(INADDR_ANY)};
	bind(soc, (struct sockaddr*)&add, sizeof(struct sockaddr));
	listen(soc, 5);
	int sz = sizeof(struct sockaddr);
	while(1){
		struct sockaddr_in cli = {0};
		int cfd = accept(soc, (struct sockaddr*)&cli, &sz);
		pthread_t read, write;
		pthread_create(&read, NULL, reader, (void*)(intptr_t)cfd);
		pthread_create(&write, NULL, writer, (void*)(intptr_t)cfd);
	}
	return 0;
}
