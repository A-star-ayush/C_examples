#include <stdio.h>
#include <stdlib.h>

int comp(const void* a, const void *b)
{
	int first=*(int*)a;          // carefully note this step [since void pointers cannot be directly derefrenced]
	int second=*(int*)b;

	if(first<second)                       
		return 1;
	else if(first==second)  // or directly return (*(int*)a - *(int*)b)
		return 0;
	else
		return -1;
}

/*Return value meaning:
<0 The element pointed by a goes before the element pointed by b
0  The element pointed by a is equivalent to the element pointed by b
>0 The element pointed by a goes after the element pointed by b
*/

int main(int argc, char const *argv[])
{

   int arr[10]={1,5,2,4,3,0,1,2,5,8};

   qsort(arr,10,sizeof(int),comp);  // comp is a function which tells it how to sort 

   int i;

   for(i=0;i<10;i++)
   printf("%d ",arr[i]);
     
     puts("");
	
	return 0;
}