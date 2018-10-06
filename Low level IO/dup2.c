#include <fcntl.h>
#include <unistd.h>  // for STDOUT_FILENO  
#include <stdio.h>
#include <stdlib.h>  // for the exit function
#include <string.h>

// The following program demonstrates the use of dup2 in redirection of the input file
/* Note this method is just to demonstrate different options and is not efficient.
    A more efficient way cud have been to read three characters at a time. This way
     you wudn't have to clear a long buffer every time. On a match to NF: its position
      cud have been saved and later lseek cud have been used to get back there. */

int main(int argc, char const *argv[])
{
	int in = open("filex1", O_RDONLY);
	
	if(in==-1){
		printf("Error: %m\n"); // printing the error message corresponding to the errno
		exit(1);                  // using perror is a better choice (didn't know it then!)
	}
		// Read Buffer
	char Rbuff[BUFSIZ];  // BUFSIZ for reading in chunks of optimum size
	int j=1;
	while(j++<5){
		
		int k=0;
		for(k=0;k<BUFSIZ;k++)   
			Rbuff[k]=0;  // clearing the buffer before reading the new content
		             // VERY IMPORTANT otherwise the NF pattern from the previous file cud be present

		ssize_t x = read(in, Rbuff, BUFSIZ);
		if(x==0)
			break;  // EOF has been reached
		char *newFile = strstr(Rbuff, "NF:");  // returns the pointer to the beginning of the substring
		if(newFile!=NULL){
			int i=3;// starting after NF:
			while(newFile[i]!=';') i++; // Eg. NF:example.txt;
			const int len = i-3+1;  // +1 to add the null character (important)
			if(len>0){
				char fname[len];  
				for(i=i-1;i>=0;i--) //Decreementing i by 1 to point to the char before ';'
					fname[i-3]=newFile[i];  // fname will then be: example.txt
				fname[len-1]='\0';
				int next_in = open(fname, O_RDONLY);
				if(next_in!=-1) 
					dup2(next_in, in);  // next_in replaces in and in is closed
					
				else{
					printf("Error: %m\n"); // printing the error message corresponding to the errno
					exit(2);
				}
			}
			else{
				puts("Length of New file is zero.\n");
				exit(3);
			}
		}
	
		else
			write(STDOUT_FILENO, Rbuff, x);  // writing the content of the RBuffer to the stdout
					// when no more NF: pattern in found
	}
	return 0;
}