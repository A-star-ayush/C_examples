#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*	One way of thinking of this recursive call would be to think about its iterative counterpart
	(although that shouldn't be the practise) and the variables (count and maxCount) it will use */

int seq(char* str, char c, int count, int maxCount){
	if(str[0]=='\0') return maxCount;  // notice \n is also the part of the string will yields one extra call	
							// but is useful when the string is ending with the character forming the sequence
	
	if(str[0]==c) seq(str+1, c, ++count, maxCount);   // count++ will yield 0 always

	else {
		if(count>maxCount) maxCount=count;
		count=0;
		seq(str+1, c, count, maxCount);
	}
}

int main(int argc, char const *argv[])
{
	printf("String: ");
	size_t n=10;
	char *str=(char*)malloc(n+1);
	getline(&str, &n, stdin);
	__fpurge(stdin); 
	printf("Character: ");
	char c;
	scanf("%c",&c);
	
	printf("Length of the longest sequnce of %c: %d\n",c,seq(str, c, 0, 0));
	return 0;
}