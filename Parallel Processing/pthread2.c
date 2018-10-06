#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>   // for strtoul
#include <unistd.h>   // for getopt
#include <string.h>  // for strdup
#include <ctype.h>   // for toupper

// link with -pthread

/* A program that creates a number of threads with the provided stack size or a default value and prints the top of
    the stack */

// error checking has been skipped for brevity

typedef struct thread{
	pthread_t tid;
	int tnum;   // Application defined thread number [can be very useful for debugging]
	char* argv_string;  // data to be handled by the thread
} thread;

void *display(void* arg){
	thread* current = arg;
	char *uargv, *p;
	printf("Thread %d; The top of the stack %p; argv_string = %s\n", current->tnum, &p, current->argv_string); // &p
	            // The top of the stack is the address of p since it is last variable created
	uargv = strdup(current->argv_string);  // duplicates the string
	for(p=uargv; *p!='\0'; p++)
		*p = toupper(*p);
	return uargv;
}

int main(int argc, char *argv[])
{
	int stacksize = -1;
	int opt;  // note it is int but still we can switch it for a character
	while((opt=getopt(argc, argv, "s:"))!=-1){  // provide the size in hexadecimal for clear understanding
		switch(opt){
			case 's': stacksize = strtoul(optarg, NULL, 0);  // converts optarg to unsigned long integer
					  break;                                    // (NULL means the entire string)
			default : puts("Usuage ./pthread2 -s size args");
					  exit(EXIT_FAILURE); 
		}
	}

	int nthreads = argc - optind;   // optind is the index of the next element to be proceesed

	thread arr[nthreads];

	pthread_attr_t attribute;
	pthread_attr_init(&attribute);

	if(stacksize>0)
		pthread_attr_setstacksize(&attribute, stacksize);

	int i;

	for(i=0; i<nthreads; i++){
		arr[i].tnum = i+1;
		arr[i].argv_string = argv[optind +i];  // we need not perform a strcpy here 
		pthread_create(&arr[i].tid, &attribute, display, &arr[i]);
	}

	pthread_attr_destroy(&attribute);  // destroying the thread attribute object since it is no longer needed

	void *res;  // needed for capturing the result of joining
	for(i=0; i<nthreads; i++){
		pthread_join(arr[i].tid, &res);  // by default the threads are joinable
		printf("Joined with thread %d, and the value returned was %s\n", arr[i].tnum, (char*)res);
		free(res);  // note this step [we free res because it might have been allocated memory for
						// capturing the return value or it might point to a memory location]
	}
	
	return 0;
}