#include <stdio.h>
#include <string.h>

struct one
{
	int a;
	int b;
	char c[100];
};

/*struct one eg1
{                        // Such sort of extended initialiser is allowed in c++ but not in c
  4,5,"Hello"          // Mark no comma after "Hello"
};
*/

struct one eg1 = {   // member specific initialization [very useful]
	.a=5,
	.b=6,				// need not initialise all the members of the struct
};

struct one eg;

struct two
{
    int a;
    int b;
};

struct two eg3;

int main(int argc, char const *argv[])
{
	eg.a=5;               // This is how it is done in c
	eg.b=4;
	strcpy(eg.c,"Hello");

	struct one eg5={10,16,"HI"};  // perhaps the best way to initialize the structure
	
	struct two eg2;
	
	printf("%d\n",eg2.a);  // prints garbage value since the object is local.
    printf("%d\n",eg3.a);  // 0  since the object is global.

	return 0;
}

/* Suppose you have a struct with a single integer field and a function that returns an integer.
   You cannot directly assign the return value from the function to the struct without surrounding
   the return value with {}. Example: struct xyz = { func() }; */