#include <stdio.h>
#include <string.h>

int isPalindrome(char* str){
	const int len=strlen(str);
	if(len<=1)							// Base case: empty string and string of length 1 are both palindromic
		return 1;
	
	char first=str[0];
	                                          // for a more efficient implementation see palindrome2.c
	char sub[len-1];
	int i;
	for(i=1;i<len-1;i++)              // creating a substring that excludes the first and the last character
		sub[i-1]=str[i];
	sub[i-1]='\0';						// Note the addition of \0 at the last index to terminate the string
	int mid=isPalindrome(sub);					// a necessary addition without which the program causes errors
	
	char last=str[len-1];
													// Inductive definition:
	return (first==last) && mid;   // a string f s l is palindromic is f==l and s is a palindrome itself
}

int main(int argc, char const *argv[])
{
	char str[10];
	printf("String (max 10 characters): ");
	scanf("%10s",str);

	if(isPalindrome(str))
		printf("It is a palindrome.\n");
	else
		printf("It is not a palindrome.\n");
	
	return 0;
}