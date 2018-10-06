#include <stdio.h>


void merge(int arr[], int aux[], int low, int mid, int high){

	int i=low;   // beginning of the first sorted  subarray
	int j=mid+1; // beginning of the second sorted subarray
	int k;
	for(k=low;k<=high;k++)
		aux[k]=arr[k];

	for(k=low;k<=high;k++){
		if(i>mid) arr[k]=aux[j++];
		else if(j>high) arr[k]=aux[i++];
		else arr[k]=aux[i]<aux[j]?aux[i++]:aux[j++];
	}

}

void sort_m(int arr[], int aux[], int low, int high){

	if(high<=low)
		return;
	int mid=(high+low)/2;
	sort_m(arr,aux,low,mid);
	sort_m(arr,aux,mid+1,high);
	merge(arr,aux,low,mid,high);
}

void sort(int arr[], int len){
	int aux[len];
	sort_m(arr,aux,0,len-1);  // remember not to copy the arr values into aux here
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
