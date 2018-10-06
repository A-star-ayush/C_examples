#define _GNU_SOURCE   // for TEMP_FAILURE_RETRY (needs to defined before including others)
#include <fcntl.h>   // try linking with -lrt when getting compilation issues
#include <sys/uio.h>
#include <unistd.h>   // BUFSIZ for optimum buffer size
#include <sys/mman.h>   // sysconf(_SC_PAGESIZE) to know the page size
#include <aio.h>
#include <errno.h>  // if errno is used explicity (TEMP_FAILURE_RETRY also uses it)
#include <stdio.h>
#include <string.h>

#define IC_AIO_DATA_INIT(_aio, _fd, _dat, _len, _off)\
  {memset(&_aio, 0, sizeof(_aio)); \
   _aio.aio_fildes = _fd;          \
   _aio.aio_buf    = _dat;         \
   _aio.aio_nbytes = _len;         \
   _aio.aio_offset = _off;}

// memset is very essential. Without it things do not work out

int main(int argc, char const *argv[])
{
	struct aiocb wptr[2];  
	struct aiocb *const aio_list[2]={&wptr[0], &wptr[1]};
								// MUST be an array of constant pointer for lio_listio to use it !! 
	
	int fdes = TEMP_FAILURE_RETRY(open("kx", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR));

	IC_AIO_DATA_INIT(wptr[0], fdes, "rec#1\n", strlen("rec#1\n"), 0)

	IC_AIO_DATA_INIT(wptr[1], fdes, "rec#2\n", strlen("rec#2\n"),wptr[0].aio_nbytes)
	
	wptr[0].aio_lio_opcode = wptr[1].aio_lio_opcode = LIO_WRITE;

	if(lio_listio(LIO_NOWAIT, aio_list, 2, 0)==0)
		puts("All operations enqueued without error.");
	else
		puts("Some error occured while enqueueing the opearations.");

	printf("%m\n");  // prints the last error message corresponding to the errno

	while(aio_error(&wptr[0])!=0);

	while(aio_error(&wptr[1])!=0);
	
	return 0;
}