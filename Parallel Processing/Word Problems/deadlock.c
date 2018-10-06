#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// A sample program to simulate deadlock.
// link with -pthread

// the deadlock here can be resolved by eliminating the "hold and wait", i.e.
// releasing the already owned resource before requesting a new one

pthread_mutex_t m1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t m2 = PTHREAD_MUTEX_INITIALIZER;

void* res1(void*);
void* res2(void*);

int main(int argc, char const *argv[])
{
	pthread_t t1, t2;
	pthread_create(&t1, NULL, res1, (void*)1);
	pthread_create(&t2, NULL, res2, (void*)2);

	pthread_join(t1, NULL);
	pthread_join(t2, NULL);

	puts("Reached the end of the program.");

	pthread_mutex_destroy(&m1);
	pthread_mutex_destroy(&m1);
	
	return 0;
}

void* res1(void* x){
	pthread_mutex_lock(&m1);
	printf("Thread %d has locked the resource m1\n", (int)x);
	res2(x);  // requesting the second resource while still holding the first resource
	pthread_mutex_unlock(&m1);

	pthread_exit(NULL);
}

void* res2(void *y){
	pthread_mutex_lock(&m2);
	printf("Thread %d has locked the resource m2\n", (int)y);
	res1(y);
	pthread_mutex_unlock(&m2);

	pthread_exit(NULL);
}