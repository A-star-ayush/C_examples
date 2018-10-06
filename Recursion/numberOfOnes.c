#include <stdio.h>

int ones(int* arr, int size){
	if(size==0)
		return 0;
	if(arr[0]==1)
		return 1+ones(arr+1,size-1);
	else
		return ones(arr+1,size-1);
}

int main(int argc, char const *argv[])
{
	printf("Size of the array: ");
	int n;
	scanf("%d",&n);
	int arr[n];
	printf("Enter the elements: \n");
	int i;
	for(i=0;i<n;i++)
		scanf("%d",&arr[i]);

	printf("Numer of ones: %d\n",ones(arr, n));
	return 0;

}