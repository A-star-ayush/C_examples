#include <stdio.h>
#include <sys/statvfs.h>
#include <stdlib.h>  // for the exit function
#include <fcntl.h>

#define exit_on_error(msg) \
	do { perror(msg); exit(EXIT_FAILURE); } while(0)

int main(int argc, char const *argv[])
{
	int fileDes = open("txx", O_RDONLY | O_CREAT);  // need not specify the mode always (two argument variant also exists)
	if(fileDes==-1)
		exit_on_error("open");

	struct statvfs info;          // vfs - virtual file system (layer of abstraction to hide the internal implementation)
	if(fstatvfs(fileDes, &info)==-1)  // returns the filesystem information
		exit_on_error("fstatvfs");

	printf("Size: %ld GB\n", (info.f_bsize*info.f_blocks)/(1024*1024*1024));
	printf("Filesystem block size: %ld\n", info.f_bsize);
	printf("Fragment size: %ld\n", info.f_frsize);
	printf("Blocks: %ld\n", (long)info.f_blocks);
	printf("Free Blocks: %ld\n", (long)info.f_bfree);
	printf("Free Blocks for unprivileged users: %ld\n", (long)info.f_bavail);
	printf("Inodes: %ld\n", (long)info.f_files);
	printf("Free Inodes: %ld\n", (long)info.f_ffree);
	printf("Free Inodes for unprivileged users: %ld\n", (long)info.f_favail);
	printf("Filesystem ID: %ld\n", info.f_fsid);
	printf("Maximum filename length: %ld\n", info.f_namemax);

	return 0;
}
