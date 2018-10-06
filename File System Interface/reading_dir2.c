#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

typedef struct dirent dirent;

#define error_handler(msg) \
	do { perror(msg); exit(EXIT_FAILURE); } while(0)

int main(int argc, char const *argv[])
{
	dirent** namelist; 
	int ele = scandir("./", &namelist, NULL, alphasort);  

	if(ele>=0){
		printf("No of entries: %d\n", ele);
		int i;
		for(i=0;i<ele;i++)
			printf("%s\n", namelist[i]->d_name);
	}
	
	else
		error_handler("scandir");

	return 0;
}