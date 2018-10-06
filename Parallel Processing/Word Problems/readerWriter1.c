#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>

// link with -pthread

// solution to the 1st variant of the reader writer problem
// for problem statement refer to OS @ VIT / Saved Webpages/ Reader ..

/* Constraint: no reader shall be kept waiting if the share is currently opened for reading */

/* In this solution, every writer must claim the resource individually. This means that a stream of readers can 
   subsequently lock all potential writers out and starve them. This is so, because after the first reader locks the 
   resource, no writer can lock it, before it gets released. And it will only be released by the very last reader. 
   Hence, this solution "does not satisfy fairness." */

// Conclusion: Reader favouring algorithm

#define N 5
#define N 5  

pthread_t readers[N];
pthread_t writers[N];

sem_t resource;
sem_t mutex;  // mutex for reader registration [entry and exit section]

volatile int readCount = 0;

int Data = 0;
int seed = 0;

void* w(void*);
void* r(void*);

int main(int argc, char const *argv[])
{
	int i,j;

	for(i=0;i<N;i++){	
		sem_init(&resource, 0, 1);
		sem_init(&mutex, 0, 1);
		puts("");
		pthread_create(&writers[i], NULL, w, (void*)i);
		pthread_join(writers[i], NULL);
		
		for(j=0;j<N;j++)
			pthread_create(&readers[j], NULL, r, (void*)j);
		
		for(j=0;j<N;j++)
			pthread_join(readers[j], NULL);
		
		
		sem_destroy(&resource);
		sem_destroy(&mutex);
	}
	
	return 0;
}

void* w(void *id){
	const int no = (int)id;

	sem_wait(&resource);
	srand(seed);
	Data  = rand() % 10;
	seed = rand();
	printf("Writer %d wrote %d\n", no, Data);
	sem_post(&resource);	

	return NULL;
}

void* r(void *id){
	
	const int no = (int)id;
	
	sem_wait(&mutex);
	readCount++;
	if(readCount == 1)  // if its the first thread, lock the resource from the writer
		sem_wait(&resource);
	sem_post(&mutex);
	
	printf("Reader %d read %d\n", no, Data);
	
	sem_wait(&mutex);
	readCount--;
	if(readCount == 0)  // if all the readers have read the data, release the resource for the writer to hold
		sem_post(&resource);
	sem_post(&mutex);
	
	return NULL;
}

