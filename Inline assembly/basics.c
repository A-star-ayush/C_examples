#include <stdio.h>
#include <math.h>

// As a reference for assembly instructions (intel) refer to Intel Software Developer Manual Vol2 
// But beware that the instructions in the guide follow intex syntax. Make appropriate changes to convert them in AT&T
// or compile with -masm=intel or cover the asm under ".intel_syantax" and ".att_syntax"

static inline char * strcpy(char * dest,const char *src){
int d0, d1, d2;
__asm__ __volatile__(  "1:\tlodsb\n\t"
                       "stosb\n\t"
                       "testb %%al,%%al\n\t"
                       "jne 1b"
                     : "=&S" (d0), "=&D" (d1), "=&a" (d2)
                     : "0" (src),"1" (dest) 
                     : "memory");
return dest;
}

int array[3] = {111, 222, 333};

#define mov_blk(src, dest, numwords) \
	__asm__ __volatile__ ("rep movsl" : "+S" (src), "+D" (dest), "+c" (numwords) : : "memory");

// "+" specifier used for both input and output (this is the only way to express an input parameter as being modified by asm)
// This version of mov_blk differs from the one taught in GCC-Inline-Assembly-HOWTO.html
// This one is correct. For explaination refer to: Others/rep_movsl.html [read the comments as well]
// The same is to be reffered for proper explaination about "clobbered list"

int main(int argc, char const *argv[])
{

	/* Always remember that in general assembly both the operands cannot be memory operands */
    /* Also if u wish to use the operands by some name instead of %0, %1, so on.. see example "set/reset a bit" */
    /* It is quite useful to define these assembly inline codes as macros. Checkout mov_blk*/

	// transfering value
	/*int a=10, b;

	asm("movl %1, %%eax;"  // ';' is used as a delimiter to separate out two assembly instructions [\n cud also be used]
		"movl %%eax, %0;"
		:"=r"(b)  // output   .. r means let GCC chose any register for it (output needs to be a lvalue)
		:"r" (a)  // input                                      .. (no such restriction on input. Can be an expression as well)
		: "%eax" // list of clobbered registers [need not mention the registers specified for output]
		);

	printf("%d\n",b);*/

	//using intel syntax
	/*int temp=0;

    asm
    (   ".intel_syntax;"
        "mov %0, 1;"
        ".att_syntax;" // important to switch back because by deafult the rest of the code is compiled to AT&T syntax
        : "=r"(temp)
        :                   
    );
    printf("temp=%d\n", temp);*/

	//using intel syntax with an array [compile with -masm=intel]
	/*int out = 0;
	__asm__ __volatile__ (
							"lea eax, [array] \n\t"
							"mov eax,[eax] \n\t"
							"inc eax \n\t"
							: "=a" (out)
							: [array] "m" (*array)
						);


	printf("out = %i\n", out);
	*/

    // multiplication by 5
	/*int x=6;

	asm("leal (%%ecx, %%ecx, 4), %%ecx;"
		:"=c"(x)
		:"c"(x)    // .. using the same register and operand for input and output
		);                   // nothing in the clobbered list since we already described ecx in output
	printf("%d\n",x);*/

	//adding two numbers
	/*int x = 5, y = 7, c;

	__asm__ __volatile__ ("addl %%ebx, %0;"
						  :"=r"(c)
						  :"0"(x), "b"(y));  // using matching constraints (same register for one input and output)
	printf("%d\n",c);*/

	// adding an immediate to a variable
	/*int a1=3, a2=3, a3=3;
	#define OP 156
	asm volatile ("lock;"   // used to ensure atomicity. If atomicity not requried, remove this statement
				  "addl %3, %0;"
				  "addl %4, %1;"
				  "addl %5, %2;"
				 :"=m" (a1), "=m" (a2), "=m"(a3)
				 :"ir" (156), "ir" (153+3), "ir" (OP)
				 );  
	printf("a1: %d a2: %d a3: %d\n",a1, a2, a3);  // results in the same value: 159
	#undef OP
	// ir means take the immediate and put it into any general purpose register
	// note: nothing gets clobbered*/

	// perform action and compare the value (operating a mutex)
	/*int mutex = 1; char cond = 0; // char important because sete instruction works on a byte (in c not type as byte exists)
	asm volatile ("lock;"
		          "decl %0;"
		          "sete %1;"  // set if the above instruction results in a zero (equality)
		          :"=m"(mutex), "=q" (cond)
		          :"m" (mutex)
		          :"memory"   // memory in the clobbered list since it was modified
		          );
	if(cond)
		puts("mutex locked");
*/

	// set/reset a bit
	/*int mask=0, pos=10;
	asm volatile ("btsl %[pos], %[mask];"   // to reset btrl can be used
				 :[mask] "=m" (mask)
				 :[pos] "Ir" (pos)   // I enforces that the value of pos be between 0 and 31
				 :"cc"   // condition codes get clobbered
				 );
	if(log2(mask) == pos)           // compile with -lm
		puts("Successfully set.");*/
	
	// string copy
	/*const char *one = "Ayush";
	char two[5];
	strcpy(two, one);
	printf("%s\n", two);*/

	//move bulk
	/*int arr1[4] = {1,2,3,4};
	int arr2[4];
	int n = 4;

	int *a1 = arr1, *a2 = arr2;  // did not use arr1, arr2 directly because the address of an array is not a lvalue

	mov_blk(a1, a2, n);
	int i;
	for(i=0;i<4;++i)
		printf("%d ", arr2[i]);
	puts("");*/


	return 0;
}