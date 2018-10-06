#include <stdio.h>
#include <cpuid.h>  // using the standard one and not the one in this folder

// get processor identification and features using the CPUID instruction
// note: the same information can be obtained by reading /proc/cpuinfo (/proc/meminfo for memory) in linux

// The cpuid.h has been downloaded from github. You can view the source to understand more.
// For the one on our pc checkout: /usr/lib/gcc/x86_64-linux-gnu/4.8/include/cpuid.h

// The ID flag (bit 21) in the EFLAGS register indicates support for the CPUID instruction (AMD always has it)
// for an example on checking that and other features checkout "Intel processor identification and CPUID instruction.pdf"
// under "Intel Software Developer Manual". For theory check the same. For breif description: Intel Vol2.pdf pg 243-273)

int main(int argc, char const *argv[])
{
	// using the functions in cpuid.h
	/*int CPUInfo[4] = {-1};
	if(__get_cpuid_max(0, NULL) >=1){  // the function returns the maximum input value to CPUID for basic/extended information
		__cpuid(1, CPUInfo[0], CPUInfo[1], CPUInfo[2], CPUInfo[3]);   // here we are using it as a gateway to check whether
        if(CPUInfo[2] & bit_SSSE3)									// we can actually perform "feature detection"
        	puts("Processor supports SSE3 extension");
        else
        	puts("Processor does not support SSE3 extension");
	}*/

	// using extended inline assembly
	/*int eax;
	char ebx[4], ecx[4], edx[4];  // remember u can interpret an array of 4 chars as an integer
	int i;
	
	// SAMPLE RUN
	for(i=0;i<3;++i){
		asm volatile("cpuid" : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx) : "0"(i));
		printf("\ninput: %d\n", i);
		printf(" eax: %d\n ebx: %d\n ecx: %d\n edx: %d\n", eax, *(int*)ebx, *(int*)ecx, *(int*)edx);
	}  

	//  Some intrepretation of the numbers (for a complete list checkout the intel manuals)
	i = 0;
	asm volatile("cpuid" : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx) : "0"(i));
	printf("\nMaximum Input value for basic CPUID information: %d\n", eax);
	printf("Vendor String: %.4s %.4s %.4s\n", ebx, ecx, edx);  // using .4 since the strings are not null terminated

	++i; int features;
	asm volatile("cpuid" : "+a"(i), "=c"(features) : : "%ebx", "%edx");  // note: clobber list
	if(features & 0x01)
		puts("\nProcessor supports SSE3 technology\n");
	else
		puts("\nProcessor does not support SSE3 technology\n");*/
	

	return 0;
}