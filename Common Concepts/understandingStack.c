#include <stdio.h>
#include <stdlib.h>

int x; /* static storage */

void main() {
 int y;
 char *str; /* dynamic stack storage */
 /* dynamic stack storage */
 str = malloc(100);
 y = foo(23);
 printf("%d\n",y);  // OUTPUT: 3
 free(str);
 }

int foo(int z) {
 char ch[100];

/* allocates 100 bytes of dynamic heap storage */
/* deallocates 100 bytes of dynamic heap storage */
/* y and str deallocated as stack frame is popped */
/* z is dynamic stack storage */
/* ch is dynamic stack storage */
if (z == 23) foo(7);
return 3;
/* z and ch are deallocated as stack frame is popped,
3 put on top of stack */
}