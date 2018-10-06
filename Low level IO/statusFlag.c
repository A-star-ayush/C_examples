#include <fcntl.h>   // try linking with -lrt when getting compilation issues


int set_nonblock_flag(int des, int val){  // des - file descriptor

	int oldflags=fcntl(des,F_GETFL,0);

	if(oldflags==-1)   // error occured
		return oldflags;

	if(val!=0)
		oldflags |= O_NONBLOCK;
	else
		oldflags &= ~O_NONBLOCK;

	return fcntl(des, F_SETFL, oldflags);  // applying the changes
}																	// Retrying if unsuccessful

int main(int argc, char const *argv[])
{
	
	return 0;
}