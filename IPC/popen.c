#include <stdio.h>
#include <stdlib.h>


int main (int argc, const char* argv[]) {
 
  FILE *ps_pipe;
  FILE *grep_pipe;

  int bytes_read;
  size_t nbytes = 100;
  char *my_string;

  /* Open our two pipes */
  ps_pipe = popen ("ps -A", "r");
  grep_pipe = popen ("grep init", "w");

  /* Check that pipes are non-null, therefore open */
  if ((!ps_pipe) || (!grep_pipe))
    {
      fprintf (stderr, "One or both pipes failed.\n");
      return EXIT_FAILURE;
    }

  /* Read the entire output from ps_pipe */
  my_string = (char *) malloc (nbytes + 1);
  bytes_read = getdelim (&my_string, &nbytes, '\0', ps_pipe); // using '\0' in this manner helps read the entire 
                                      // output at once assuming that there are no intermediate null bytes in the file

  /* Close ps_pipe, checking for errors */
  if (pclose (ps_pipe) != 0) fprintf (stderr, "Could not run 'ps', or other error.\n");
    
  /* Send output of 'ps -A' to 'grep init', with two newlines */
  fprintf (grep_pipe, "%s", my_string);

  /* Close grep_pipe, checking for errors */
  if (pclose (grep_pipe) != 0) fprintf (stderr, "Could not run 'grep', or other error.\n");
    
  /* Exit! */
  return 0;
}