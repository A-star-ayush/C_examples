#include <fcntl.h>  // for the low level operations and the O_* constants
#include <stdlib.h>  // for exit
#include <stdio.h>  // for puts

int main(int argc, char const *argv[])
{
	if(argc<3){
	 puts("Must supply the input and the output file."); exit(1); }
	
	int in=open(argv[1], O_RDONLY); 
	 
	if(in==-1){
	  puts("Invalid input file."); exit(2); }
	
	int out=open(argv[2], O_WRONLY | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);  // format open(file, flags, modes)
					// in modes u can even specify an octal number like 0666 to specify the file permissions
	if(out==-1){
		out=open(argv[2], O_WRONLY| O_APPEND);   // append if the file with the output name already exists

		if(out==-1){
			puts("Some error occured in opening the output file."); exit(3); }
	}
	

	char buff[BUFSIZ];

	while(1){
		ssize_t x=read(in, &buff, BUFSIZ);
		if(x==0)
			break;  // EOF has been reached

		write(out, &buff, x);   // note x may not always be BUFSIZ, 
									//therefore we must not write in chunks of BUFSIZ
	}

	return 0;
}