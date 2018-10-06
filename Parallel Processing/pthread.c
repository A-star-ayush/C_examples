#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct thread{
	pthread_t tid;
	int tnum;
	char *str;
};

typedef struct thread thread;

void* on_thread_start(void* arg){
	thread* ptr = arg;
	printf("Thread id :%d Arg: %s\n", ptr->tnum, ptr->str);
	return NULL;
};

int main(int argc, char const *argv[])
{
	thread arr[3];
	int i;
	for(i=0;i<3;i++){
		arr[i].tnum = i;
		arr[i].str = "Hello";

		pthread_create(&arr[i].tid, NULL, on_thread_start, &arr[i]);
	}

	for(i=0;i<3;i++){
		pthread_join(arr[i].tid, NULL);
	}
	return 0;
}