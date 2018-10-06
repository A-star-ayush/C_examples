#include <stdio.h>
#include <stdlib.h> // for rand() and srand() and exit
#include <pthread.h>
#include <string.h>  // for memset()

/* The producer and the consumer share a common fixed size buffer. The problem is to make sure that 
   the producer won't try to add data if the buffer is full and that the consumer won't try to remove
   data from an empty buffer. This is also known as the "bounded buffer problem" */

/* We make use of mutexes to avoid a "deadlock". Also the data order is inconsistent (on purpose). It is a solution
   for single producer and single consumer problem.
   Checkout PC_v2.c for a solution that uses "monitors". Data order is consistent and can be extended to 
   multiple producers and consumers. */

#define NBytes 10
#define QSize 4
#define err_handler(msg) \
   do { perror(msg); exit(EXIT_FAILURE);} while(0)


void* produce(void*);
void* consume(void*);

char queue[QSize];  
int qIndex = 0;
volatile int itemCount = 0;
pthread_mutex_t mCount = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char const *argv[])
{
	int i; 
	pthread_t prod, cons;  // the producer and the consumer thread
	char input[NBytes], output[NBytes];

	srand(100);  // 100 is the seed value. Change it to get a new sequence of random numbers.
	for(i=0; i<NBytes; i++){
		input[i]= 65 + (rand() % 26);  // so that it always produces alphabets only
	}

	memset(output, '#', NBytes);  // initializing the output array with all #'s
	memset(queue, '!', QSize);  
	
	if(pthread_create(&prod, NULL, produce, (void*)input)!=0)
		err_handler("pthread_create [Producer]");

	if(pthread_create(&cons, NULL, consume, (void*)output)!=0)
		err_handler("pthread_create [Consumer]");

	if(pthread_join(prod, NULL)!=0)
		err_handler("pthread_join [Producer]");

	if(pthread_join(cons, NULL)!=0)
		err_handler("pthread_join [Consumer]");

	
	for(i=0;i<NBytes;i++)
		printf("%c ", input[i]);
	puts("");
	for(i=0;i<NBytes;i++)
		printf("%c ", output[i]);
	puts("");
	for(i=0;i<QSize;i++)
		printf("%c ",queue[i]);
	puts("");
	return 0;
}

void* produce(void* x){
	char* input = x;  // you cannot straight ahead use x because a void* pointer cannot be derefrenced
	int index = 0;

	while(index!=NBytes){
		while(itemCount == QSize);
		pthread_mutex_lock(&mCount);
		queue[qIndex++] = input[index++];
		itemCount++;
		pthread_mutex_unlock(&mCount);
	}
	
	return NULL;
}

void* consume(void* y){
	char* output = y;
	int index = 0;

	while(index!=NBytes){
		while(itemCount == 0);
		pthread_mutex_lock(&mCount);
		output[index++] = queue[--qIndex];
		itemCount--;
		pthread_mutex_unlock(&mCount);
	}

	return NULL;
}



// There are others ways to tackle this problem [refer to the wikipedia page saved under CSE222]
// Specifically, as described on that page, the "FIFO" or "channel" is really efficient for a 
// single producer and consumer
