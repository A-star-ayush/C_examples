#include <stdio.h>

typedef struct   //1st style
{
	int a=5; // Such sort of initialization is not supported with gcc. Use g++ instead.
	int b,c,d=7,e,f;
}test;

struct xxx
{
   int a=6;
   int b,c,d=7,e,f;
};

typedef struct xxx test2; //2nd style [more time efficient]

int main(int argc, char const *argv[])
{
	test one;
	printf("%d %d\n",one.a,one.d);

	test2 two;
	printf("%d %d\n",two.a,two.d);
	
	return 0;
}