#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/uio.h>  // needed for writev function
#include <string.h>

int main(int argc, char const *argv[])
{
	char buf1[10], buf2[10], buf3[10], buf4[10];

 	scanf("%s",buf1);
 	scanf("%s",buf2);
 	scanf("%s",buf3);
 	scanf("%s",buf4);

 	struct iovec output_vector[]={ 
 						   {buf1, strlen(buf1)},   // if u simply supply 10 instead of strlen(buf) u may get weird 
 						   {buf2, strlen(buf2)},       // results because scanf need not fill the entire buffer
 						   {buf3, strlen(buf3)},    // but writev will write each buffer upto the specified length
 						   {buf4, strlen(buf4)},         // before moving to the next buffer
 						};

 	int file=open("try", O_WRONLY | O_CREAT | O_EXCL , S_IRUSR | S_IWUSR);
 	
	if(file==-1){
	 puts("Some error occured while opening the file for output. May be a file with the same name already exists.");
	 exit(1);
	}

	writev(file, output_vector, 4);

	char buf5[10], buf6[10], buf7[10], buf8[10];

	struct iovec input_vector[]={
								{buf5,10},
								{buf6,10},
								{buf7,10},
								{buf8,10},
							};

	close(file);

	// lseek(file,0,SEEK_SET);  // going back to the starting of the file

	file=open("try", O_RDONLY);

	if(file==-1){
	 puts("Some error occured while opening the file for input.");
	 exit(1);
	}

	readv(file, input_vector, 4);

	puts("");
	puts("The content of the input buffers is as follow: ");

	puts(buf5);
	puts(buf6);
	puts(buf7);
	puts(buf8);

	return 0;
}