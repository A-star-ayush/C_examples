#include <stdio.h>
#include <pthread.h>
#include <stdint.h>

void* on_thread_start(void* arg){
	int x = (intptr_t)arg;
	printf("thread was called with: %d\n", x);
	return NULL;
}

int main(int argc, char const *argv[])
{
	int i = 5;
	pthread_t t;
	int s = pthread_create(&t, NULL, on_thread_start, (void*)(intptr_t)i);
	pthread_join(t,NULL);	
	return 0;
}