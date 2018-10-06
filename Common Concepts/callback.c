#include <stdio.h>
#include <stdlib.h>

/*First of all the name is misleading. There is no "callback" in C - not more than any other generic programming concept.
They are implemented using simple function pointers.
*/

void populate_array(int *array, size_t arraySize, int (*getNextValue)(int))
{	
	size_t i;
    for (i=0; i<arraySize; i++)
        array[i] = getNextValue(10);
}

int getNextRandomValue(int m)  // mark the way the arguments are passed and handled
{
    return rand() % m;  // not a good way to generate random numbers
}

int main(void)
{
    int myarray[10];
    populate_array(myarray, 10, getNextRandomValue);
    int i;
    for(i=0;i<10;i++)
    	printf("%d ",myarray[i]);
    printf("\b\n");
}
