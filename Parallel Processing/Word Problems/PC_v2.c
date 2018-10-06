#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Checkout ProducerConsumer.c

// pthread_cond_t cond_recv = PTHREAD_COND_INITIALIZER;
// pthread_cond_t cond_send = PTHREAD_COND_INITIALIZER;
// pthread_mutex_t cond_mutex = PTHREAD_MUTEX_INITIALIZER; // used as the associated mutex with the wait condition  
pthread_mutex_t full_mutex = PTHREAD_MUTEX_INITIALIZER;  // used for establishing mutual exclusion on the res		

int full = 0;
int count = 0;

void* produce(void* x){
	while(1){
		// pthread_mutex_lock(&cond_mutex);
		while(full){
			// pthread_cond_wait(&cond_recv, &cond_mutex); // its like saying I will wait and give away my exclusive
		}						// access on the mutex when the condition is met/received via a broadcast
		// pthread_mutex_unlock(&cond_mutex); // because upon successful return from wait, it will be locked again
		pthread_mutex_lock(&full_mutex);
		count++; full = 1;
		printf("Produced: %d\n", count);
		// pthread_cond_broadcast(&cond_send);  
		pthread_mutex_unlock(&full_mutex);

		if(count>=10)
			break;
	}
}

/* All the commented pieces of code above and below are not necessary for a single producer and consumer
   scenario. But if multiple of them exists then they are needed as well. */

void* consume(void* y){
	while(1){
		// pthread_mutex_lock(&cond_mutex);
		while(!full){
			// pthread_cond_wait(&cond_send, &cond_mutex);  // wait (remain blocked) till the condition is met
		}									
		// pthread_mutex_unlock(&cond_mutex);
		pthread_mutex_lock(&full_mutex);
		full = 0;
		printf("Consumed: %d\n", count);
		// pthread_cond_broadcast(&cond_recv);
		pthread_mutex_unlock(&full_mutex);

		if(count>=10)
			break;
	}
}


int main(int argc, char const *argv[])
{
	pthread_t prod1, cons1;
	// pthread_t prod2, cons2;

	pthread_create(&prod1, NULL, produce, NULL);  // error-checking skipped for the brevity of the example
	// pthread_create(&prod2, NULL, produce, NULL);  
	pthread_create(&cons1, NULL, consume, NULL);
	// pthread_create(&cons2, NULL, consume, NULL);


	pthread_join(prod1, NULL);
	// pthread_join(prod2, NULL);	
	pthread_join(cons1, NULL);
	// pthread_join(cons2, NULL);

	return 0;
}
