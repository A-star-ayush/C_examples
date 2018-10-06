#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>
#include <grp.h>

int main(int argc, char const *argv[])
{
	uid_t id = getuid();  // id now contains the real id of the user

	struct passwd* x = getpwuid(id);  // fetching the entry from the password database usind id as the key
	if(x==NULL) { perror("getpwuid"); exit(EXIT_FAILURE); }
	
	puts("Info about the current user:");
	printf("Name: %s\n", x->pw_name);
	printf("Password: %s\n", x->pw_passwd);  // if "x" then the password is present in /etc/shadow
	printf("User ID: %d\n", x->pw_uid);
	printf("Group ID: %d\n", x->pw_gid);
	printf("Home dir: %s\n", x->pw_dir);
	printf("Default shell: %s\n", x->pw_shell);
	printf("Other info: %s\n", x->pw_gecos);

	/* Getting group info*/

	struct group* y = getgrgid(x->pw_gid); // we need not use getgid since we already have that information in x
	if(y==NULL) { perror("getgrgid"); exit(EXIT_FAILURE); }

	printf("Group Name: %s\n", y->gr_name);
	printf("Group password: %s\n", y->gr_passwd);
	printf("Group members: ");
	int i;
	for(i=0; y->gr_mem[i]; ++i)
		printf("%s", y->gr_mem[i]);
	puts("");

	return 0;
}
