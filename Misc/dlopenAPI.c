// the dlopen API consists of dlopen, dlclose, dlerror and dlsym which implement the interface to the 
// dynamic linking loader

/*If this program were in a file named "foo.c", you would build the  program  with
       the following command:

           gcc -rdynamic -o foo foo.c -ldl   // lnking with -ldl

       Libraries  exporting  _init()  and  _fini() will want to be compiled as follows,
       using bar.c as the example name:

           gcc -shared -nostartfiles -o bar bar.c
*/

// EXAMPLE [loading the math library and finding the cosine]:

// Refer to the wikipedia page saved about dynamic loading to see how its different from linking and
           // other details

#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

int main(int argc, char const *argv[])
{
	void *handle = dlopen("libm.so", RTLD_LAZY); // (libname, mode) for more information checkout man dlopen
	if(!handle){ //dlopen returns the handler to the libary [relative paths cud have been provided] or NULL(error)
		fprintf(stderr, "%s\n",dlerror());  // dlerror returns the error since its last call
		exit(EXIT_FAILURE);
	}

	double (*cosine)(double);  // a function pointer	
	dlerror();  // clearing any existing error

	*(void**)(&cosine) = dlsym(handle, "cos"); // returns the memory address of the symbol 
	/*	 Writing: cosine = (double (*)(double)) dlsym(handle, "cos");
     would seem more natural, but the C99 standard leaves casting from "void *" to a function pointer undefined.
     The assignment used above is the POSIX.1-2003 (Technical Corrigendum 1) workaround;
	*/

     if(dlerror()!=NULL){
     	fprintf(stderr, "%s\n",dlerror());
     	exit(EXIT_FAILURE);
     }

     printf("%f\n",(*cosine)(20));  // computing the cosine of 2 using the function pointer obtained
     dlclose(handle);
     exit(EXIT_SUCCESS);
	
	return 0;
}