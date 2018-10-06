#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

// A general priority driven scheduling (where the priorities are externally provided)
// It is non preemptive. So cannot be used in timesharing environment.
// Very similar to sjf (the only difference is "based on what you are sorting")

typedef struct PCB{
	int pid, atime, btime, start, finish, isdone, prio;
}PCB;

int on_externalPriority(const void* a, const void* b){
	const PCB* x = a;
	const PCB* y = b;

	return x->prio - y->prio;  // can implement it in reverse (if higher value means lesser priority)
}

int min(int x, int y){
	int r = y ^ ((x^y) & -(x<y));
	return r;
}

int main(int argc, char const *argv[])
{
	int n, i;

	float avg_wait = 0.0, avg_turn = 0.0;

	printf("No of processes: ");
	scanf("%d",&n);
	printf("\nFill in the following table:\n");
	printf("\nPID\tArrival Time\tBurst Time\tPriority\n");

	PCB arr[n];
	memset(arr, (char)0, sizeof(PCB)*n);

	for(i=0;i<n;++i)
		scanf("%d %d %d %d",&arr[i].pid, &arr[i].atime, &arr[i].btime,&arr[i].prio);

	qsort(arr, n, sizeof(PCB), on_externalPriority);

	printf("\nThe scheduling results in:\n");

	int t = INT_MAX;   // simulating the time 
	for(i=0;i<n;++i)
		t = min(t, arr[i].atime);   // starting the time from the earliest arrival

	int rounds = n;
	while(rounds--){     // will exactly take n loops to schedule all the processes
		int i=0;
		// skip over any process that has already completed or is yet to arrive 
		while(arr[i].isdone==1  || arr[i].atime > t ) i++; 
		
		arr[i].start = t;
		arr[i].finish = arr[i].start + arr[i].btime;
		arr[i].isdone = 1;

		avg_wait += arr[i].start - arr[i].atime;
		avg_turn += arr[i].finish - arr[i].atime;

		t+=arr[i].btime;   // updating the time

		printf("Process %d consumed %d units of time.\n", arr[i].pid, arr[i].btime);
	}
	
	avg_wait /= n;
	avg_turn /= n;

	printf("Average waiting time: %.2f\n", avg_wait);
	printf("Average turnaround time: %.2f\n", avg_turn);  // .2 used to print upto two decimal places only

	return 0;
}
