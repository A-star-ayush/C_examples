#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// fcfs = Fist Come First Serve [ Non preemptive. Thus not suitable for timesharing environment where response time
// must be guaranteed ]

typedef struct PCB {
	int pid, atime, btime, start, finish;
}PCB;

int on_arrivalTime(const void* a, const void* b){
	const PCB* x = a;
	const PCB* y = b;   // don't drop off the const

	return x->atime - y->atime;
}

int main(int argc, char const *argv[])
{
	int n, i;

	float avg_wait = 0.0, avg_turn = 0.0;

	printf("No of processes: ");
	scanf("%d",&n);
	printf("\nFill in the following table:\n");
	printf("\nPID\tArrival Time\tBurst Time\n");

	PCB arr[n];
	memset(arr, (char)0, sizeof(PCB)*n);

	for(i=0;i<n;++i)
		scanf("%d %d %d",&arr[i].pid, &arr[i].atime, &arr[i].btime);

	qsort(arr, n, sizeof(PCB), on_arrivalTime);

	printf("\nThe scheduling results in:\n");

	for(i=0;i<n;++i)
		printf("Process %d consumed %d units of time.\n", arr[i].pid, arr[i].btime);
	printf("\n");

	for(i=1;i<n;++i){
		arr[i].start = arr[i-1].start + arr[i-1].btime;
		arr[i].finish = arr[i].start + arr[i].btime;
		avg_wait += arr[i].start - arr[i].atime;   // waiting time = start - arrival
		avg_turn += arr[i].finish - arr[i].atime;  // turnaround time = finish - arrival
	}
	
	avg_turn += arr[0].btime;   // first process to come in has no waiting time but contributes to average turnaorund time

	avg_wait /= n;
	avg_turn /= n;

	printf("Average waiting time: %.2f\n", avg_wait);
	printf("Average turnaround time: %.2f\n", avg_turn);  // .2 used to print upto two decimal places only

	return 0;
}
