#define _GNU_SOURCE 1
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/uio.h>
#include <sys/mman.h>   // sysconf(_SC_PAGESIZE) to know the page size

#define FILENAME        "/tmp/foo"
#define NUM_THREADS     3
#define ITERATIONS      5


/* Example of using open file description locks in a threaded program. 
   If this program used process-associated locks, then it would be subject to data corruption 
   because process-associated locks are shared by the threads inside a process, and thus cannot be 
   used by one thread to lock out another thread in the same process. */


// compile and link with -pthread

void* thread_start (void *arg)
{
  int i, fd, len;
  long tid = (long) arg;
  char buf[256];
  struct flock lck = {
    .l_whence = SEEK_SET,
    .l_start = 0,
    .l_len = 1,
  };

  fd = open ("/tmp/foo", O_RDWR | O_CREAT, 0666);

  for (i = 0; i < ITERATIONS; i++)
    {
      lck.l_type = F_WRLCK;
      fcntl (fd, F_OFD_SETLKW, &lck);

      len = sprintf (buf, "%d: tid=%ld fd=%d\n", i, tid, fd);

      lseek (fd, 0, SEEK_END);
      write (fd, buf, len);
      fsync (fd);

      lck.l_type = F_UNLCK;  //Remember to unlock the region of file once your 
      fcntl (fd, F_OFD_SETLK, &lck); //work is done otherwise other processes will not be able to use it.

      /* sleep to ensure lock is yielded to another thread */
      usleep (1);
    }
  pthread_exit (NULL);
}

int main (int argc, char **argv)
{
  long i;
  pthread_t threads[NUM_THREADS];

  truncate (FILENAME, 0);

  for (i = 0; i < NUM_THREADS; i++)
    pthread_create (&threads[i], NULL, thread_start, (void *) i);

  pthread_exit (NULL);
  return 0;
}
