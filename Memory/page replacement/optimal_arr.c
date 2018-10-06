#include <stdio.h>
#include <stdlib.h>

// optimal means looking ahead and checking which page will not be used for the longest time

int ind=0;

void print(int* arr, int sz){
	int i;
	for(i=0;i<sz;++i){
		if(arr[i]==-1)
			break;
		printf("%d ",arr[i]);
	}
	printf("\n");
}

int search(int* arr, int sz, int val){
	int i;
	for(i=0;i<sz;++i){
		if(arr[i]==-1){
			ind = i;
			return -2;
		}

		if(arr[i]==val)
			return 1;
	}
	return -1;
}

int main(int argc, char const *argv[])
{
	int n, i, j, k, nframes, hits=0, counter=0;
	puts("");
	printf("No of frames: ");
	scanf("%d", &nframes);
	printf("No of pages in the sequence string: ");
	scanf("%d", &n);

	int seq[n];
	puts("Enter the sequence: \n");
	for(i=0;i<n;++i)
		scanf("%d",&seq[i]);

	int frames[nframes]; int encountered[nframes];

	for(i=0;i<nframes;++i)
		frames[i] = -1;   // -1 indicating that the frame is empty
	
	

	for(i=0;i<n;++i){
		int rt = search(frames, nframes, seq[i]);  // check whether the page exists in our frame table
		if(rt==-2)
			frames[ind] = seq[i];

		else if (rt==1)
			hits++;
			
		else{

			for(j=0;j<nframes;++j)
				encountered[j] = 0;
			counter=0;

			for(j=i+1;j<n;++j){
				if(counter==nframes-1)
					break;
				
				for(k=0;k<nframes;k++){
					if(frames[k] == seq[j]){
						if(encountered[k] != 1){
							encountered[k]=1;
							++counter;
						}
					}
				}
			}
		
			for(j=0;j<nframes;++j){
				if(encountered[j]==0){
					frames[j] = seq[i];
					break;
				}
			}
		}
		print(frames, nframes);
	}

	puts("");
	printf("No of hits: %d\n", hits);
	return 0;
}