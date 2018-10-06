#define _GNU_SOURCE  // to use FTW_ACTIONRETVAL and the corresponding return values
#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <ftw.h>
#include <stdlib.h>

#define error_handler(msg) \
	do { perror(msg); exit(EXIT_FAILURE); } while(0)

int processEntry(const char* name, const struct stat* info, int flag, struct FTW *details){
	int i;
	for(i=1;i<details->level;++i) printf("\t");  // level indicates at the which level in the hirearchy are we (0 for root)
	printf("%s\n",name+details->base); // adding the offset of 'base' to the complete name in order to print the entry name only
	return FTW_CONTINUE;
}

int main(int argc, char const *argv[])
{
	if(argc!=2){
		printf("Usage %s <root directory>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	if(nftw(argv[1], processEntry, 20, FTW_ACTIONRETVAL)==-1)  // 3rd arg is the maximum no of open file descriptors
		error_handler("ftw");  // can specify FTW_DEPTH for depth first search

	exit(EXIT_SUCCESS);
}