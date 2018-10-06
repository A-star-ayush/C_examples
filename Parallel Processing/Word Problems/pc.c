#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t full_mutex = PTHREAD_MUTEX_INITIALIZER;  // used for establishing mutual exclusion on the res		

int full = 0;
int count = 0;

void* produce(void* x){
	while(1){
		while(full);				// access on the mutex when the condition is met/received via a broadcast
		pthread_mutex_lock(&full_mutex);
		count++; full = 1;
		printf("Produced: %d\n", count);
		pthread_mutex_unlock(&full_mutex);

		if(count>=10)
			break;
	}
}


void* consume(void* y){
	while(1){
		while(!full);						
		pthread_mutex_lock(&full_mutex);
		full = 0;
		printf("Consumed: %d\n", count);
		pthread_mutex_unlock(&full_mutex);

		if(count>=10)
			break;
	}
}


int main(int argc, char const *argv[])
{
	pthread_t prod1, cons1;

	pthread_create(&prod1, NULL, produce, NULL);  // error-checking skipped for the brevity of the example
	pthread_create(&cons1, NULL, consume, NULL);

	pthread_join(prod1, NULL);
	pthread_join(cons1, NULL);

	return 0;
}
