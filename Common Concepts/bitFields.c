#include <stdio.h>

/*A bit field is a term used in computer programming to store multiple, logical, neighboring bits, 
where each of the sets of bits, and single bits can be addressed. 
*/

/*In C, we can specify size (in bits) of "structure and union members" (cannot be used with normal variables). The 
idea is to use memory efficiently when we know that the value of a field or group of fields will never exceed a 
limit or is withing a small range. 
*/

// Bit-fields are thought to be nonportable, although they are no less portable than other parts of the language. Endianness affects bit fields. 
// Checkout Guides/Hardware\ Pertaining/Endianness (summary: little endian system store bit fields from LSB wherease big endian systems do it from MSB)
// It will tell you have to write portable code involving endianness

// Checkout ArrayOfBits.c seemingly similar but a different method

struct date{
	unsigned int d:5;  // 5 bits since dates will vary only between 0-31
	unsigned int m:4;  // format 'type member_name: width'   type must be int or _Bool (c99)
	unsigned int y;   // a normal variable

};

struct test1{
	unsigned int x:5;
	unsigned int y:8;
};

struct test2{
	unsigned int x:5;
	unsigned int:0;   // special unnamed bit field of 0 to force alingment on next addressable boundary 
	unsigned int y:8;   // checkout the size difference below
};

static struct t3{   // static structs are allowed
	int x:5;
	// static int y:6;  // but static bit fields are not
}; 							// also array of bit fields in not allowed

int main(int argc, char const *argv[])
{
	
	struct date one={24,9,2014};
	struct date two={35,20,2014};  // generates a warning since we are trying to cross the limits

	printf("%d %d %d\n",one.d, one.m, one.y);
	printf("%d %d %d\n",two.d, two.m, two.y);  // prints 3 (35-32) for two.d and 4 (20-16) for two.m 
	                                             // The above behaviour is implementation dependent

	printf("%d\n", sizeof(struct test1));  // 4
	printf("%d\n", sizeof(struct test2));  // 8

	// unsigned int* ptr= &one.x;   // Error. There cannot be pointers to bit field members 
									// since they may not have start at a byte boundary

	return 0;
}
