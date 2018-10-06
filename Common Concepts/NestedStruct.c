#include <stdio.h>

struct one
{
	int c;
	struct three{  //Warning: Declaration does not define anything
     int k;
	};

	struct three instance1;
	int d;
};

struct two
{
	int a;
	int b;

  struct one instance1;  //two has an instance of one but one couldnot have of two becoz it was declared before.
};

int main(int argc, char const *argv[])
{
	struct two instance1;
	instance1.a=5;
	instance1.instance1.c=2; // legal although the name of the identifiers seem to match but scope differs.
    instance1.instance1.instance1.k=5;  //worked fine
    printf("%d\n",instance1.instance1.instance1.k);
  
	return 0;
}