#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* The  program below uses fmemopen() to open an input buffer, and open_memstream() to open a dynamically sized 
	output buffer.  The program scans its input string (taken from the  program's  first  command-line  argument)  
	reading  integers, and writes the squares of these integers to the output buffer.  An example of the output 
	produced by this program  is  the following:

           $ ./a.out '1 23 43'
           size=11; ptr=1 529 1849 */

#define handle_error(msg) \
           do { perror(msg); exit(EXIT_FAILURE); } while(0)

int main(int argc, char *argv[])   // note that its not char const *argv[]
{
	if(argc<2){
		fprintf(stderr, "Usuage: %s <file>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	FILE *in, *out;

	in = fmemopen(argv[1], strlen(argv[1]), "r");
	if(in == NULL)
		handle_error("fmemopen");

	char *ptr;
	size_t sz;

	out = open_memstream(&ptr, &sz);
	if(out == NULL)
		handle_error("fmemopen");

	int s, val;

	while(1){
		s = fscanf(in, "%d ",&val);
		if(s<=0)
			break;
		s = fprintf(out, "%d ", val*val);
		if(s==-1)
			handle_error("fprintf");
	}	

	fclose(in);
	fclose(out);   // once out is closed or flushed, sz points to the current length of the buffer and ptr
												// points to the buffer
	fprintf(stdout, "size = %ld; ptr=%s\n", (long)sz, ptr);  
	free(ptr);
	return 0;
}

