#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>


// error checking skipped for brevity
int main(int argc, char *argv[])
{
	int soc = socket(PF_INET, SOCK_STREAM, 0);
	struct sockaddr_in add;
	add.sin_family = AF_INET;
	add.sin_port = htons(30000);
	inet_aton("127.0.0.1", &add.sin_addr);
	connect(soc, (struct sockaddr*)&add, sizeof(struct sockaddr_in));

	int i;
	for (i = 0; i < 2; ++i)
		write(soc, "HELLO", 5);  // write HELLO twice normally
	sleep(1);  // sleeping for a second to ensure that the ordinary segment and the out-of-band segement
			   // are well distinguished for the sake of the example

	send(soc, "U", 1, MSG_OOB);  // send "U" as out-of-band data
								 // You can send only 1 byte as OOB. If you send "EXP" as OOB, only the last byte
								 // 'P' will be considered out of band

	sleep(1);
	for (i = 0; i < 2; ++i)
		write(soc, "HELLO", 5);  // write HELLO twice normally

	close(soc);

	return 0;
}
