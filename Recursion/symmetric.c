#include <stdio.h>
#include <stdio_ext.h>

// A symmetric sequence is one in which the middle element is 0 (and that is the only 0 in the sequence)
// and the sequence is palindromic

int isSymmetric(int* arr, int n){
	int first=arr[0];
	if(first==0)
		return !n;
	else
	{
		int mid=isSymmetric(arr+1,n-2);  // -2 because arr has also been increemented by 1
		int last=arr[n];
		return (first==last) && mid;
	}
}

int main(int argc, char const *argv[])
{
	printf("Number of elements: ");
	int n;
	scanf("%d",&n);
	__fpurge(stdin);  // to remove any other unnecessary inputs

	printf("Input the numbers:\n");
	int i;
	int arr[n];
	for(i=0;i<n;i++)
		scanf("%d",&arr[i]);
	
	if(n%2==0)   // because 0 is in middle and the two sides are symmetric
		puts("The sequence of numbers is not symmetric.");
	else
	{	
		if(isSymmetric(arr,n-1))  
			puts("The sequence of numbers is symmetric.");
		else
			puts("The sequence of numbers is not symmetric.");
	}

	return 0;
}