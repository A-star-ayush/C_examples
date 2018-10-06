#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
	
	char buffer[100000];
	printf("How many strings will you be entering?\n");
	int num;
	scanf("%d",&num);
	char *arr[num];
	int counter=0;
	while(counter<num)
	{
		scanf("%s",buffer);
		arr[counter]=(char *)malloc(strlen(buffer));
		strcpy(arr[counter++],buffer);  
	}
	
   for(counter=0;counter<num;counter++)
   	printf("%s\n",arr[counter]);
	return 0;
}