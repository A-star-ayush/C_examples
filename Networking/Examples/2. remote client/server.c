#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

// error checking skipped for brevity

int main(int argc, char const *argv[])
{
	int hSoc = socket(PF_INET, SOCK_STREAM, 0);
	struct sockaddr_in sadd= { AF_INET, htons(20000), htonl(INADDR_ANY) };
	bind(hSoc, (struct sockaddr*)&sadd, sizeof(struct sockaddr_in));
	listen(hSoc, 5);

	while(1){
		struct sockaddr_in cadd = {0};
		int sz = sizeof(struct sockaddr_in);
		int cfd = accept(hSoc, (struct sockaddr*)&cadd, &sz);
		write(cfd, "MAST..BHAIIIIIIIIII", 18);
		close(cfd);
	}	

	return 0;
}
