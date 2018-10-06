#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// link with -pthread

/* A simple program that creates and exits threads */

void *PrintHello(void *id){  // mark the function definition (only a single argument of type void* and return void*)
	printf("Thread %ld says Hello.\n", (long)id);  // note the way it is casted to long without being derefrenced
	pthread_exit(NULL); // this implicity gets called on performing a return from the start function (here PrintHello)
}

int main(int argc, char const *argv[])
{
	pthread_t threads[5];  // note no strcut was used here [ can be used as any other datatype; such a declaration is
																	// is provided in the header file ]
	int i;
	for(i=0;i<5;i++){
		printf("In main, creating thread no: %d.\n",i);
		if(pthread_create(&threads[i], NULL, PrintHello, (void*)i)!=0)   // &i was not used to get the pointer
			printf("Some error occured during the creation of thread no %d.\n", i);
	}						// Also arrays and structures or anything for that matter can be passed to pthread_create
	
	pthread_exit(NULL);	  
	return 0;
}