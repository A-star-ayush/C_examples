/*     The  following  program changes the ownership of the file named in its second command-line
       argument to the value specified in its first command-line argument.  The new owner can  be
       specified  either  as a numeric user ID, or as a username (which is converted to a user ID
       by using getpwnam(3) to perform a lookup in the system password file). */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pwd.h>

#define error_handler(msg) \
        do { perror(msg); exit(EXIT_FAILURE); } while(0)

int main(int argc, const char *argv[]) {
    
    uid_t uid;
    struct passwd *pwd;
    char *endptr;

    if (argc != 3 || argv[1][0] == '\0') {
      fprintf(stderr, "Usage: %s <owner> <file>\n", argv[0]);
      exit(EXIT_FAILURE);
    }

    uid = strtol(argv[1], &endptr, 10);   /* Allow a numeric string */
    // note the style of using endptr
    if (*endptr != '\0') {                /* Was not pure numeric string */
      pwd = getpwnam(argv[1]);            /* Try getting UID for username */
      if (pwd == NULL) error_handler("getpwnam");
      uid = pwd->pw_uid;
    }
      
    if (chown(argv[2], uid, -1) == -1) error_handler("chown");

    exit(EXIT_SUCCESS);
}
