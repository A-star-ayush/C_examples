#include <fcntl.h>

/* Set the FD_CLOEXEC flag of desc if val is nonzero,
   or clear the flag if val is 0.
   Return 0 on success, or -1 on error with errno set. */

int set_cloexec_flag(int des, int val){  // des- file descriptor

	int oldflags=fcntl(des,F_GETFD,0);

	if(oldflags<0)   // error has occured
		return oldflags;

	if(val!=0)
		oldflags |= FD_CLOEXEC;  // sets the specific flag
	else
		oldflags &= ~FD_CLOEXEC;   // clears the specific flag

	return fcntl(des, F_SETFD, oldflags);   // applying the new flags
}

int main(int argc, char const *argv[])
{
	
	return 0;
}