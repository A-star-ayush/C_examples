#include <stdio.h>
#include <stdlib.h>

// fifo = first in first out

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
		if(arr[i]==-1)
			return 0;
		if(arr[i]==val)
			return 1;
	}
	return 0;
}

int main(int argc, char const *argv[])
{
	int n, i, nframes, hits=0, index=0;
	puts("");
	printf("No of frames: ");
	scanf("%d", &nframes);
	printf("No of pages in the sequence string: ");
	scanf("%d", &n);

	int seq[n];
	puts("Enter the sequence: \n");
	for(i=0;i<n;++i)
		scanf("%d",&seq[i]);

	int frames[nframes];
	for(i=0;i<nframes;++i)
		frames[i] = -1;   // -1 indicating that the frame is empty

	for(i=0;i<n;++i){
		if(search(frames, nframes, seq[i]))  // check whether the page exists in our frame table
			hits++;
		else{
			frames[index] = seq[i];
			index = (index+1)%nframes;
		}
		print(frames, nframes);
	}

	puts("");
	printf("No of hits: %d\n", hits);
	return 0;
}