#include <fcntl.h>
#include <sys/uio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <aio.h>
#include <stdio.h>

// link with -lrt

int main(int argc, char const *argv[])
{
	int out = open("x_wr", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
	
	const int len = 5000000; 
	char output[len];
	int i; char c='a';
	for(i=0; i<len; i++){
		if(c == 'z'+1)
			c='\n';
		output[i]=c;   // writing lines of abcd...z separated by \n
		if(c=='\n')
			c='a'-1;
		c++;
	}
	
	struct aiocb wptr;
	wptr.aio_fildes = out;  // note that there is no "e" after l in fildes
	wptr.aio_buf = output;
	wptr.aio_offset = 0;
	wptr.aio_nbytes = len;
    struct sigevent asig;
    asig.sigev_notify = SIGEV_SIGNAL; 
    wptr.aio_sigevent = asig;

    int x = aio_write(&wptr);
    if(x==0)
    	puts("Writing operation successfully enqueued");

    for(i=0;i<100;i++){    // Meanwhile doing something else
    	if(i%10==0)
    		printf("%d\n", i);
    	else
    		printf("%d ", i);
    }
    puts("");
    while(aio_error(&wptr)!=0);  // beacuse the program should not exit before everything has been written
	printf("aio_returned %d\n",(int)aio_return(&wptr));  // number of bytes written
	
	printf("aio_returned %d\n",(int)aio_return(&wptr));  // the second time it returns -1
	
	return 0;
}

