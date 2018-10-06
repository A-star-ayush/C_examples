#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <stdint.h>

// link with -pthread

void* reader(void *_fd){
	pthread_detach(pthread_self());
	int fd = (intptr_t)_fd;
	char rmsg[BUFSIZ];
	while(1){
		if(read(fd, rmsg, BUFSIZ)>0)
			printf("%s", rmsg);
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
	struct sockaddr_in add = { AF_INET, htons(20000), inet_addr("127.0.0.1")};
	connect(soc, (struct sockaddr*)&add, sizeof(struct sockaddr_in));
	pthread_t read, write;
	pthread_create(&read, NULL, reader, (void*)(intptr_t)soc);
	pthread_create(&write, NULL, writer, (void*)(intptr_t)soc);
	
	pthread_join(read, NULL);
	pthread_join(write, NULL);

	return 0;
}
