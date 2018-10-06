#include <stdio.h>
#define true 1
	
int partition(int arr[], int low, int high){
	int i=low;
	int j=high+1;
	while(true){
		while(arr[++i]<arr[low]){
			if(i==high)
				break;
		}
		while(arr[--j]>arr[low]){
			if(j==low)
				break;
		}
		if(i>=j)
			break;
		int temp=arr[i];
		arr[i]=arr[j];
		arr[j]=temp;
	}
	int temp=arr[j];
	arr[j]=arr[low];
	arr[low]=temp;
}

void sort_q(int arr[], int low, int high){
	if(high<=low)
		return;
	int j=partition(arr,low,high);
	sort_q(arr,low,j-1);
	sort_q(arr,j+1,high);
}

void sort(int arr[], int len){
	sort_q(arr,0,len-1);
}

int main(int argc, char const *argv[])
{
	printf("Size: ");
	int n;
	scanf("%d",&n);
	printf("Enter %d elements:\n",n);
	int arr[n];
	int i=0;
	for(i;i<n;i++)
		scanf("%d",&arr[i]);
	sort(arr,n);

	for(i=0;i<n;i++)
		printf("%d ",arr[i]);
	printf("\n");
	return 0;
}