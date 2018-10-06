#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

// link with -pthread

// for a description of the problem checkout "Dining philosophers problem" on wikipedia [the page might have
// have been saved under OS @ VIT ]

// This program demonstrates the deadlock condition which is reached upon following the naive algorithms:
  
  /* Think until the left fork is available; when it is, pick it up;
     Think until the right fork is available; when it is, pick it up;
     When both forks are held, eat for a fixed amount of time;
     Then, put the right fork down;	
     Then, put the left fork down;
     Repeat from the beginning. */


pthread_mutex_t forks[5];
sem_t arb;

void* on_thread_start(void*);

int main(int argc, char const *argv[])
{
	pthread_t threads[5];

	int i;
     
    sem_init(&arb,0,1); 
	for(i=0;i<5;i++)
		pthread_mutex_init(&forks[i], NULL);

	for(i=0; i<5;i++)
		pthread_create(&threads[i], NULL, on_thread_start, (void*)i);

	for(i=0;i<5;i++)
		pthread_join(threads[i], NULL);

	return 0;
}

void* on_thread_start(void* id){
	const int no = (int)id;
	while(1){
		sem_wait(&arb);
		printf("Philosopher %d is thinking.\n", no);
		pthread_mutex_lock(&forks[no]);
		printf("Philosopher %d owns fork no %d\n", no, no);
		pthread_mutex_lock(&forks[(no+1) % 5]);   // so the 5th philosopher requests the 0th fork 
		printf("Philosopher %d is eating.\n", no);
		pthread_mutex_unlock(&forks[(no+1) % 5]); 
		pthread_mutex_unlock(&forks[no]);
		sem_post(&arb);
   }
}

    