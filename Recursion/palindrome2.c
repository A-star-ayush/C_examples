#include <stdio.h>
#include <string.h>

// Yet another way to check for palindrome is to reverse the string and then compare the original
// and the reversed string using strcmp

int isPalindrome(char* str, int n){
	if(n < 1)                      // a single character alone on itself is palindromic
		return 1;
	char first=str[0];
	int mid=isPalindrome(str+1,n-2);   // note the use pointer arithmetic and "n-2" and not 1
	char last=str[n];                        // the reason for -2 is that str has also been increemented by 1

	return (first==last) && mid;   // a string f s l is palindromic is f==l and s is a palindrome itself
}

int main(int argc, char const *argv[])
{
	char str[10];
	printf("String (max 10 characters): ");
	scanf("%10s",str);

	if(isPalindrome(str,strlen(str)-1))
		printf("It is a palindrome.\n");
	else
		printf("It is not a palindrome.\n");
	
	return 0;
}