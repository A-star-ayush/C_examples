#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int foo(const char *whoami){
    printf("I am a %s.  My pid is:%d  my parent is %d ", whoami, getpid(), getppid() );
    return 1;
}

int main(void)
{
    int n= 10;
    int i=0;
    int status=0;

    int x = 5;

    printf("Creating %d children\n", n);
    foo("parent");
    puts("");
    for(i=0;i<n;i++){
        pid_t pid=fork();    
        if (pid==0){  /* only execute this if child */
            foo("child");
            for(i=0; i< 5; i++);
            x++;
            printf("x: %d\n", x);    
            exit(0);    
        }
        else if(pid>0) wait(&status);  /* only the parent waits */
        else { fprintf(stderr, "Error occured while forking\n"); exit(EXIT_FAILURE); }
    }

    x = 100 * x;
    printf("x value in main: %d\n", x);
    
    return 0;
}

/* Note this use of fork will result in sequential execution of the child processes beacuse the parent waits
    before creating a new children. However if u wish a concurrent execution, remove the wait statement from the for loop
    calling fork and place it in another for loop.*/