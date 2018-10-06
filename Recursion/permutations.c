#include <stdio.h>
#include <string.h>

// see anagrams.java for a better version ... this is not working correctly

void printArray(char* str, int n){
	int i=0;
	for(;i<n;i++)
		printf("%c",str[i]);  // printing the permutation
	printf("\n");
}

void permut(char* str, int n, int i){
	printArray(str, n);
	int j;
	for(j=i+1;j<n;j++){
		char temp=str[i];
		str[i]=str[j];
		str[j]=temp;

		permut(str,n,i+1);

		char temp2=str[i];
		str[i]=str[j];
		str[j]=temp2;
	}
}

int main(int argc, char const *argv[])
{
	char str[10];
	
	printf("Enter a string (max length 10): ");
	scanf("%10s",str);

	printf("The permutations are as follows: \n");

	permut(str,strlen(str),0);  // 0 meaning print all permutations starting from 0th index
	
	return 0;
}