#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <unistd.h>


int main(int argc, char const *argv[])
{
	if(argc<2){
		printf("Usuage: %s <number of cpus>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	cpu_set_t* set1;
	int ncpus = atoi(argv[1]);

	set1 = CPU_ALLOC(ncpus);
	if(set1 == NULL){
		perror("CPU ALLOC: ");
		exit(EXIT_FAILURE);
	}

	size_t sz = CPU_ALLOC_SIZE(ncpus);  // basically returns how much size is required for the given no of CPUs

	CPU_ZERO_S(sz, set1);

	int i;

	for(i=0; i<ncpus; i+=2)
		CPU_SET_S(i, sz, set1);

	printf("CPU count: %d\n", CPU_COUNT_S(sz, set1));
	printf("Size required: %d\n", (int)sz);

	CPU_FREE(set1);
	exit(EXIT_SUCCESS);

	return 0;
}
