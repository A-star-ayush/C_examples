#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

// srtf = shortest remaining time first [ Preemtive. Thus useful in timesharing environments. Has higher overhead than SJF
// Arrival of small processes will run almost immediately. However, longer jobs have even longer mean waiting time. ]

// waiting time = start - arrival   ... doesnot apply to here (or to any preemtive scheduling)
// turnaround time = finish - arrival  ... still valid but here we change the semantics of finish to lastTime
// When a process is completed lastTime will mean finish so it can be used to calculate turnaround time. 

// also semantics burst time (btime) is replaced with remaining time (rtime)

typedef struct PCB{
	int pid, atime, rtime, wtime, lastTime, isdone;
}PCB;              

int on_burstTime(const void* a, const void* b){
	const PCB* x = a;
	const PCB* y = b;

	return x->rtime - y->rtime;
}

int alldone(PCB* arr, int n){
	int i;
	for(i=0;i<n;++i)
		if(arr[i].isdone==0)
			break;
	return 1 - (i!=n);
}

int min(int x, int y){
	int r = y ^ ((x^y) & -(x<y));
	return r;
}

void reassemble(PCB* arr, int n, int index){   // performing insertion sort to bring the process to its right position
	int i = index - 1;
	while(i>=0 && arr[i].rtime >arr[i+1].rtime) --i;
	++i;
	if(i!=0){
		PCB temp = arr[i];  // Note: not PCB* temp
		arr[i] = arr[index];
		arr[index] = temp;
	}
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
		scanf("%d %d %d",&arr[i].pid, &arr[i].atime, &arr[i].rtime);  // initially remaining time = burst time

	qsort(arr, n, sizeof(PCB), on_burstTime);

	printf("\nThe scheduling results in:\n");

	int t = INT_MAX;   // simulating the time 
	for(i=0;i<n;++i)
		t = min(t, arr[i].atime);   // starting the time from the earliest arrival

	for(i=0;i<n;i++)
		arr[i].lastTime = arr[i].atime; // important (do not miss out on this)
	
	while(!alldone(arr, n)){ 
		
		int i=0;
		// skip over any process that has already completed or is yet to arrive 
		while(arr[i].isdone==1  || arr[i].atime > t ) i++; 
		
		int temp = t;  // to store the time
		
		arr[i].wtime = t - arr[i].lastTime;  // difference between the time now and the last time it was started

		while(1){
			
			if(arr[i].rtime==0)	{   // if the current process is overwith
				arr[i].isdone=1;
				printf("Process %d consumed %d units of time.\n", arr[i].pid, t - temp);
				break;
			}

			--arr[i].rtime;
			++t;

			arr[i].lastTime = t;

			int j=i-1;  // only processes before i can have smaller burst time then it
			
			while(j>=0 && (arr[j].isdone==1  || arr[j].atime > t) ) j--;
			if(j<0)
				continue;
			else{
				printf("Process %d consumed %d units of time.\n", arr[i].pid, t-temp);		
				reassemble(arr, n, i);
				break; 
			}	
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
