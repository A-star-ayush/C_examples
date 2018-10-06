#include <stdio.h>
#include <stdlib.h>  // required for the on_exit function

int on_main_exit(int , void *);

// Also checkout atexit.c [difference is that no argument could be passed to it]

int main(int argc, char const *argv[])
{
	int a = 5; 
	on_exit(on_main_exit, (void*)a);  // registers the function to be called at normal process termination
          // the function is passed the status argument given to the last call of exit(3) captured as 'int'
	                                     // and the argument from on_exit() captured as 'void *'
	printf("main method is now exiting.\n");
	return 0;
}

// the same function can be registered multiple times and will be called once for each registration

int on_main_exit(int rt, void* arg){
	printf("Return status :%d\n", rt);
	printf("Argument passed: %d\n", (int)arg);
}
