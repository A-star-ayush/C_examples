#include <stdio.h>

enum Days{
	Monday, Tuesday, Wednesday, Thursday, Friday, Saturday, Sunday,  // The last comma is optional
};

int main(int argc, char const *argv[])
{
    enum Days my_fav=Monday;  // You can assign values using numbers also	
    printf("%d\n",my_fav);  // Prints 0. These numbers are assigned by the compiler
                      // you can change the defualt values by initialising them with integer values.
                      // enums values are constant- they cannot be changed.
                      // But the pointer can be made to increement and decreement

    enum Days user;
    printf("Enter the number of your favourite day[1-7]: ");
    int input;                
    scanf("%d",&input); // Taking input for an enumerated type
    user=input-1;           // error in c++

    if(my_fav==user)   // Note the difference bw enums and strings
    	puts("Good for me.");
    else                    
    	puts("Bad for you.");

if(my_fav==Monday)
	puts("Monday is my favourite day."); //Disadv:No direct method to print in c {is there in java}
                     //Adv: enums can be used as switch case variables.
	return 0;
}


/*#include <stdio.h>  // a better form of using it

enum COLOR {
  red,
  green,
  blue
};

char *color_name[] = {
  [red]="red",
  [green]="green",
  [blue]="blue"
};

int main() {
  enum COLOR c = red;
  printf("c=%s\n", color_name[c]);
  return 0;
}*/


  // X Macros can also be used to ensure that the above method is consistent always.


//Size of enums:
  
  /*enum try{a,b,c,d,e,f,g=4294967296};
  enum try one;
  int x=sizeof(one);
  printf("%d\n",x);  // 8 : sizeof(int)*2 because one integer at max has value 4294967295
    */


  /*enum exp
{
   A,B,C='+',D='-'    // such declarations are also allowed.
};
*/
