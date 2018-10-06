#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <stdint.h>  // for intptr_t

// error checking skipped for brevity
// link with -pthread

int nclient = 0;
pthread_mutex_t nc = PTHREAD_MUTEX_INITIALIZER;

void* on_thread_start(void* _fd){
	pthread_detach(pthread_self());  // thread resources are deallocated upon return since there is no join
	int fd = (intptr_t)_fd;
	pthread_mutex_lock(&nc);
	printf("Servicing client no %d with fd: %d\n", nclient, fd);
	pthread_mutex_unlock(&nc);
	write(fd, "CHECK_START", 12);
	sleep(5);
	write(fd, "CHECK_END", 12);
	close(fd);
	return NULL;
}

int main(int argc, char const *argv[])
{
	int hSoc = socket(PF_INET, SOCK_STREAM, 0);
	struct sockaddr_in sadd= {AF_INET, htons(20000), htonl(INADDR_ANY)};
	bind(hSoc, (struct sockaddr*)&sadd, sizeof(struct sockaddr));
	listen(hSoc, 5);
	while(1){
		struct sockaddr_in cadd = {0};
		int sz = sizeof(struct sockaddr);
		int cfd = accept(hSoc, (struct sockaddr*)&cadd, &sz);
		if(cfd!=-1){
			pthread_mutex_lock(&nc);
			++nclient;
			pthread_mutex_unlock(&nc);
			pthread_t cthread;
			pthread_create(&cthread, NULL, on_thread_start, (void*)(intptr_t)cfd);
		}
	}

	return 0;
}