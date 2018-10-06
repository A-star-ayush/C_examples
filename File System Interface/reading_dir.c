#include <stdio.h>
#include <errno.h>
#include <dirent.h>

// checkout reading_dir2.c which uses a high level interface to get things done easily

extern int errno;
typedef struct dirent dirent;

int main(int argc, char const *argv[])
{
	DIR* pwd;
	pwd = opendir(".");   // opening a directory stream to the current directory
                                // error-checking skipped here
	while(1){
		errno = 0;                              // setting errno to 0 before reading
		dirent* x = readdir(pwd);                        // to help distinguish btw error and EOF
		if(x==NULL){
			if(errno == 0)                          // the entries gets listed in a random fashion
				puts("End of Directory");                               // one can sort them using array sort functions
			else                                                   // checkout Qsort.c
				puts("Encountered an error.");
		    break;
		}

		printf("%s ", x->d_name);
		printf("%s\n", (x->d_type) == DT_UNKNOWN ? "[UNKNOWN]" :
					   (x->d_type) == DT_REG ? "[REGULAR]":
					   (x->d_type) == DT_DIR ? "[DIRECTORY]":
					   (x->d_type) == DT_LNK ? "[SYMBOLIC LINK]":
					   (x->d_type) == DT_BLK ? "[BLOCK DEVICE]" : "OTHER");
		 // other possibilities have been skipped for brevity
	}
	return 0;
}