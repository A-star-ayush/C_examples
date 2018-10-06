#include <stdio.h>

// lru = least recently used

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
			return i;
	}
	return -1;
}

int main(int argc, char const *argv[])
{
	int n, i, j, nframes, hits=0;
	puts("");
	printf("No of frames: ");
	scanf("%d", &nframes);
	printf("No of pages in the sequence string: ");
	scanf("%d", &n);

	int seq[n];
	puts("Enter the sequence: \n");
	for(i=0;i<n;++i)
		scanf("%d",&seq[i]);

	int frames[nframes]; int age[nframes];

	for(i=0;i<nframes;++i){
		frames[i] = -1;   // -1 indicating that the frame is empty
		age[i] = -1;
	}

	for(i=0;i<n;++i){
		int rt = search(frames, nframes, seq[i]);  // check whether the page exists in our frame table
		if(rt==-2){
			frames[ind] = seq[i];
			age[ind] = 0;
			for(j=0;j<nframes;++j){
				if(j==rt) continue;
				else --age[j];
			}
		}

		else if (rt>=0){
			hits++;
			age[rt] = 0;
			for(j=0;j<nframes;++j){
				if(j==rt) continue;
				else --age[j];
			}
		}

		else{
			int minInd = 0;
			for(j=0;j<nframes;++j){
				if(age[j] < age[minInd])
					minInd = j;
			}
			frames[minInd] = seq[i];
			age[minInd] = 0;
			for(j=0;j<nframes;++j){
				if(j==minInd) continue;
				else --age[j];
			}
		}
		print(frames, nframes);
	}

	puts("");
	printf("No of hits: %d\n", hits);
	return 0;
}