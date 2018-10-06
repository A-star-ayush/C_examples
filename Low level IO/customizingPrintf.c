#include <stdio.h>
#include <printf.h>

struct marks
{
	int science, maths, copmuters;
};

typedef struct marks marks;

marks array[]={   // Mark the way the structures are being initialised in an array
	{78,54,89},
	{56,89,29},
	{87,37,19},
	{37,72,29},
	{92,17,60},
	};

int main(int argc, char const *argv[])
{
	register_printf_function('M',print_marks);  
	  // if the second argument (handler function) is null, all the existing handler function for arg1 is removed
	  // returns 0 is successful and -1 if not
		
	return 0;
}