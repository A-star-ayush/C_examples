#include <stdio.h>
#include <fcntl.h>
#include <sys/uio.h>
#include <unistd.h>

int input(int fileds, int seconds){
  fd_set set;
  struct timeval timeout;

  FD_ZERO(&set);           // Initialising a fd_set struct
  FD_SET(fileds, &set);

  timeout.tv_sec = seconds;  // Initialising a timeval struct
  timeout.tv_usec = 0;
                      // select returns 0 if timeout, 1 if input available, -1 if error
  return select(FD_SETSIZE, &set, NULL, NULL, &timeout); 

}

int main(int argc, char const *argv[])
{
  int rt_value = input(STDIN_FILENO,3);   // returns 0 if the user does not provide
                                                                     // anything within 3 seconds 
  if(rt_value == 0)
    fprintf(stdout, "User did not provide any input within 3 seconds.\n");
  else
  {
    char c;
    read(STDIN_FILENO, &c, sizeof(char));
    printf("The first character read is: %c\n",c);    // remember to press enter after providing the input
  }
  return 0;
}