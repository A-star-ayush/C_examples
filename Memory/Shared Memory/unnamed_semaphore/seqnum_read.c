#include "seqnum.h"

// Error checking skipped for brevity
// link with -pthread and -lrt


int main(int argc, char const *argv[])
{
	int fd = shm_open("/SHM1", O_RDWR, 0);

	struct shm_buf* ptr;
	size_t sz = sizeof(struct shm_buf);

	ptr = mmap(NULL, sz, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	close(fd);  // fd no longer required once we have mapped the SHM object to the process's address space

	sem_wait(&ptr->sem);

	printf("Current value of sequence number: %d\n", ptr->seqnum);

	if(argc>1){
		int increement  = atoi(argv[1]);
		if(increement<0)
			fprintf(stderr, "Invalid increement value\n");
		else{
			sleep(3); // make the update slow
			ptr->seqnum += increement;
			printf("Updated semaphore value\n");
		}
	}

	sem_post(&ptr->sem);

	munmap(ptr, sz);

	return 0;
}