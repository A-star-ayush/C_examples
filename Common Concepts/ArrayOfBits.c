#include <stdio.h>
#include <string.h>  // for the memset function
#include <limits.h>   // for CHAR_BIT (or 8 can be used if limits.h is unavailable)

// Array of bits can be created over an array of char or int
// The following macros will be useful to deal with such an array: [Mark the way the args are used]

#define BITMASK(b) (1 << ((b) % CHAR_BIT)) // sets the bth bit to 1 creating a bitmask; eg [000..10] if b=1
#define BITSLOT(b) ((b) / CHAR_BIT) // tells at which array index(slot) the concerned bit is
#define BITSET(a,b) ((a)[BITSLOT(b)] |= BITMASK(b))  // ORing sets the bit
#define BITCLEAR(a, b) ((a)[BITSLOT(b)] &= ~ BITMASK(b))  //And the negation of the mask to clear the bit
#define BITTEST(a, b) ((a)[BITSLOT(b)] & BITMASK(b))  //Only And tells whether the bit is set or not (no = here)
#define BITXOR(a, b) ((a)[BITSLOT(b)] ^= BITMASK(b)) // if set, clears it; if not, sets it
#define BITNSLOTS(nb) ((nb+CHAR_BIT-1) / CHAR_BIT)  // an extra CHAR_BIT to account for padding  


/*int main(int argc, char const *argv[])
{
	char bitArray[BITNSLOTS(53)];  // bit array of 53 bits (the macro tells how many slots of char will be required)
	BITSET(bitArray, 10);  // sets the 10th bit
	if(BITTEST(bitArray, 10))  // testing the 10th bit
		puts("The 10th bit is set.");
	BITXOR(bitArray, 10);
	if(!BITTEST(bitArray, 10))  // testing the 10th bit
		puts("The 10th bit is clear.");

	return 0;
}*/

// Following is a implementation of the sieve of erastosthenes using bitwise array access:

#define MAX 1000

int main(int argc, char const *argv[])
{
	char bitArray[BITNSLOTS(MAX)];

	memset(bitArray, 0, BITNSLOTS(MAX));  // good practise and is important

	int i,j;

	for(i=2; i<MAX; i++){
		if(!BITTEST(bitArray, i)){
			printf("%d ", i);
			for(j=i+i;j<MAX;j+=i)
				BITSET(bitArray, j);
		}
	}
	return 0;
}