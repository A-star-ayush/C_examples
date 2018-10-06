#include <stdio.h>
#include <stdarg.h>

int add(int count,...);

main()
{
    int ans1=add(5,1,2,3,4,5);
    int ans2=add(6,1,2,3,4,5,6);
    int ans3=add(7,1,2,3,4,5,6,7);

    printf("%d\n",ans1);
    printf("%d\n",ans2);
    printf("%d\n",ans3);
      
}

int add(int count,...)
{
	double sum=0;
	va_list arg;
	va_start(arg,count);  // the second arg is the last argument whose type is known, therefore
	int i;                                   // should't be func pointer, array, register variable
                                        // Also this function must be called before using the variable list
	for(i=0;i<count;i++)
	{
        sum+=va_arg(arg,int);  // the second arg is the type for the next argument
	}
    va_end(arg);
    return sum;
}