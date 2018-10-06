#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char const *argv[])
{	
	char str[100];
	scanf("%s", str);
	int len = strlen(str);

	pid_t rt = fork();

	if(rt==0){
		int i;
		for(i=0;i<len;i++){
			if(isdigit(str[i]))
				printf("%c ", str[i]);
		}
	}

	else if(rt>0){
		int i;
		for(i=0;i<len;i++){
			if(isalpha(str[i]))
				printf("%c ", str[i]);
		}
		wait(NULL);
	}

	else
		puts("Error");

	return 0;
}