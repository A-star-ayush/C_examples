#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

// rr = round robin [ preemptive. Thus can be used for timesharing environment. One of the most widely used. The only issue
// with it is the length of the quantum - if short, too many context switches lowering the CPU efficiency; if long, poor 
// response time and approximates to fcfs ]

// no process waits for more than (n-1)*rr time units (n- no of processes on ready queue; rr - round robin time)

// waiting time = start - arrival   ... doesnot apply to here (or to any preemtive scheduling)
// turnaround time = finish - arrival  ... still valid but here we change the semantics of finish to lastTime
// When a process is completed lastTime will mean finish so it can be used to calculate turnaround time. 


typedef struct PCB{
	int pid, atime, rtime, wtime, lastTime;
}PCB;              

int on_arrivalTime(const void* a, const void* b){
	const PCB* x = a;
	const PCB* y = b;   // don't drop off the const

	return x->atime - y->atime;
}

int alldone(PCB* arr, int n){
	int i;
	for(i=0;i<n;++i)
		if(arr[i].rtime>0)
			break;
	return 1 - (i!=n);
}

void shift(PCB* arr, int n){
	int i;
	PCB temp = arr[0];
	for(i=1;i<n;++i)
		arr[i-1] = arr[i];
	arr[--i] = temp;  // i==n at this step   
}

int main(int argc, char const *argv[])
{
	int n, i, rr;

	float avg_wait = 0.0, avg_turn = 0.0;

	printf("No of processes: ");
	scanf("%d",&n);
	printf("Round robin time: ");
	scanf("%d",&rr);
	printf("\nFill in the following table:\n");
	printf("\nPID\tArrival Time\tBurst Time\n");

	PCB arr[n];
	memset(arr, (char)0, sizeof(PCB)*n);

	for(i=0;i<n;++i)
		scanf("%d %d %d",&arr[i].pid, &arr[i].atime, &arr[i].rtime);  // initially remaining time = burst time

	qsort(arr, n, sizeof(PCB), on_arrivalTime);

	printf("\nThe scheduling results in:\n");

	int t = arr[0].atime;   // since we sorted the array based on arrival time

	for(i=0;i<n;i++)
		arr[i].lastTime = arr[i].atime;  // important (do not miss out on this)
	
	int ncopy = n;

	while(!alldone(arr, ncopy)){

		arr[0].wtime += t - arr[0].lastTime;

		if(arr[0].rtime <= rr){
			printf("Process %d consumed %d units of time.\n", arr[0].pid, arr[0].rtime);
			t+=arr[0].rtime;
			arr[0].rtime = 0;   //  necessary for the "alldone" check
			arr[0].lastTime = t;
			shift(arr, ncopy);
			ncopy--;  // by decreasing the value of n we sort of neglect the done ones.
		}	

		else{
			printf("Process %d consumed %d units of time.\n", arr[0].pid, rr);
			t+=rr;
			arr[0].lastTime = t;
			arr[0].rtime -= rr;
			shift(arr, ncopy);  
		} 
	}

	for(i=0;i<n;++i){   
		avg_wait += arr[i].wtime;
		avg_turn += arr[i].lastTime - arr[i].atime;
	}

	avg_wait /= n;
	avg_turn /= n;

	printf("Average waiting time: %.2f\n", avg_wait);
	printf("Average turnaround time: %.2f\n", avg_turn);  // .2 used to print upto two decimal places only

	return 0;
}
