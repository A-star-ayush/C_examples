#include <stdio.h>

// one can use gcc extensions to C for the same purpose

int main(int argc, char const *argv[])
{	
	long int num; long int posl, posh;
	printf("x: ");  // for posl, posh although the possible value of positions range from 0 - 63 but 
	scanf("%ld", &num);			// they also need to be of the same type as the number

	__asm__ __volatile__ ("bsfq %2, %0; bsrq %2, %1" : "=r" (posl), "=r" (posh) : "r"(num)); // suffix: 'q' for quad word (64)
									// bsf - bit scan forward  // bsr - bit scan reverse
	printf("LSB: %ld\n", posl);
	printf("MSB: %ld\n", posh);

	return 0;
}