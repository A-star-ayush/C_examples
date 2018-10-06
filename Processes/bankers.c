#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
	int pnum, rnum, i, j, k, p;

	printf("No of processes: ");
	scanf("%d",&pnum);
	printf("No of resources: ");
	scanf("%d",&rnum);

	int alloc[pnum][rnum];
	int max[pnum][rnum];
	int need[pnum][rnum];
	int avail[rnum]; int request[rnum]; int work[rnum];

	puts("Enter the allocation matrix:");
	for(i=0;i<pnum;++i){
		for(j=0;j<rnum;++j){
			scanf("%d",&alloc[i][j]);
		}
	}
	puts("Enter the max matrix:");
	for(i=0;i<pnum;++i){
		for(j=0;j<rnum;++j){
			scanf("%d",&max[i][j]);
		}
	}
	puts("Enter the available vector:");
	for(i=0;i<rnum;++i)
		scanf("%d",&avail[i]);
		
	
	printf("Enter the id for the requesting process: ");
	scanf("%d",&p);
	printf("Enter the vector of resources requested: ");
	for(i=0;i<rnum;++i)
		scanf("%d",&request[i]);
	
	for(i=p,j=0;j<rnum;++j)
		alloc[i][j] += request[j];
	
	for(i=0;i<rnum;++i)
		work[i] = avail[i] - request[i];
	
	for(i=0;i<pnum;++i){
		for(j=0;j<rnum;++j){
			need[i][j] = max[i][j] - alloc[i][j];
		}
	}

	int finish[pnum];
	for(i=0;i<pnum;++i)
		finish[i] = 0;

	while(1){
		int state=0;
		for(j=0;j<pnum;++j)
			if(finish[j]==0)
				break;
		if(j==pnum)
			break;

		for(j=0;j<pnum;++j){
			if(finish[j]==1)
				continue;
			for(i=0;i<rnum;++i)
				if(need[j][i] > work[i])
					break;
			if(i==rnum){
				state=1;
				printf("p%d ",j);
				for(i=0;i<rnum;++i)
					work[i] += alloc[j][i];
				finish[j] = 1;
			}
		}

		if(state==0){
			puts("");
			puts("Deadlock reached: No safe sequence found");
			puts("");
			exit(1);
		}
	}


	return 0;
}