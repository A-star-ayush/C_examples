#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <regex.h>
#include <unistd.h>
#include <strings.h> // for bzero
#include <sys/ioctl.h>


// ./a.out <ip> <port> <tcp/udp> <#clients> <io cycles> .. for a mix of tcp/udp one can run 2 different instances of this program simaltaneously

static inline void err(char* errmsg){
	fprintf(stderr, "%s\n", errmsg);
	exit(EXIT_FAILURE);
}

#define exit_on_error(msg) \
	do { perror(msg); exit(EXIT_FAILURE); } while(0)

#define IPERE "[0-9]\\{1,3\\}\\.[0-9]\\{1,3\\}\\.[0-9]\\{1,3\\}\\.[0-9]\\{1,3\\}"

// mark the use of \ to escape the second \ which is the part of the regular expression

int main(int argc, char *argv[])
{	

	int portNum = 20000, proto = 0, nclients = 1, seq = 0; 
	char ip[16] = "127.0.0.1";  // deafult values
	char cycles[11] = "r";  // only a single read cycle by default
	
	char c;
	while((c=getopt(argc, argv, "p:i:n:t:s"))!=-1){  // for using getopt we remove the const qualifier from argv
		switch(c){
			case 'p': errno = 0;
				  portNum = strtol(optarg, NULL, 10);
				  if(errno!=0) perror("Port Number: strtol");
				  if(portNum >= (1<<16) || portNum < 0) err("Port number should be in the range [0 65535]");
				  break;
				  // mark ';' in the next case. That is to avoid the error "a label can only be part of a statement"
			case 'i': ;regex_t ipExp; char errBuf[25];   // compiling the reg exp into a data structure for efficiency
				  int rt = regcomp(&ipExp, IPERE, REG_NOSUB | REG_EXTENDED);  // note: ERE - extended regular expression
				  if(rt!=0) { regerror(rt, &ipExp, errBuf, 25); err(errBuf); }
				  strncpy(ip, optarg, 15); ip[15]='\0';
				  rt = regexec(&ipExp, ip, 0, NULL, 0);  // since we are only interested in checking, we do not catch the   
				  if(rt!=0) err("Please provide a valid ip address of the form a.b.c.d");  // subexp that matched (REG_NOSUB)
				  regfree(&ipExp);
				  break;
			case 'n': errno = 0;
				  nclients = strtol(optarg, NULL, 10);
				  if(errno!=0) perror("#Clients: strtol");
				  break;
			case 't': if(strcasecmp(optarg, "tcp")==0);  // by default proto = 0 symbolizes tcp connection
				  else if(strcasecmp(optarg, "udp")==0) proto = 1;  // proto = 1 symbolizes udp connection (our own convention)
				  else error("Invalid protocol");
				  break;
			case 's': seq = 1;
				  break;

			default: err("Invalid option");
		}
	}

	if((argc-optind) != 0){  // meaning an i/o cycle is provided
		int l = strlen(argv[optind]);
		int min = (10^l) ^ (l & -(l>10)); // maximum 10 cycles allowed by child
		strncpy(cycles, argv[optind], min);
		cycles[min] = '\0';
	}
	
	int i;
	for(i=1;i<=nclients;++i){
		pid_t id = fork();
		if(id==0){  // Inside Child
			int M = -(proto>0);
			int type = (SOCK_DGRAM & M) | (SOCK_STREAM & ~M); 
			int soc = socket(PF_INET, type, 0);
			if(soc==-1) exit_on_error("socket");
			struct sockaddr_in sAdd;
			int sz = sizeof(struct sockaddr_in);
			bzero(&sAdd, sz);
			sAdd.sin_family = AF_INET;
			sAdd.sin_port = htons(portNum); // although using inet_addr is not advisable, we have used it here for  
			sAdd.sin_addr.s_addr = inet_addr(ip); //  portability we already checked the ip address against the regexp to    
			 		// cover up for inet_addr which returns -1 in case of error which equals 255.255.255.255 (a valid address)
					// recommended is inet_aton over inet_addr. All that checking above wouldn't be neccessary if inet_aton was used instead.
			if(connect(soc, (struct sockaddr*)&sAdd, sz)==-1) exit_on_error("connect");  // But for the purpose of learning regex, otherwise was done
			char* x = cycles;
			char buf[BUFSIZ];
			int val = 0;
			ioctl(soc, FIONBIO, &val);  // enabling blocking mode I/O on soc
			while(*x != '\0'){
				if(*x == 'r'){
					int rt = read(soc, buf, BUFSIZ); 
					if(rt == -1) perror("read");
					printf("Connection %d read %s\n", i, buf);
				}														
				else if(*x == 'w'){ 
					int rt = write(soc, "TEST", 5);
					if(rt == -1) perror("write");
					printf("Connection %d sent TEST\n", i);
				}
				++x;
			}
			close(soc);
			printf("Closed Connection #%d\n", i);
			exit(0);  // **IMP** otherwise the children will go on to create more children themselves (total 2^N-1 children)
		}
		else if(id > 0) {  if(seq) wait(NULL); }
		else { error("Error occured while creating a child process."); }
	}
	
	if(!seq)
		for(i=0;i<=nclients;++i)
			wait(NULL);  // **IMP** otherwise zombie processes will be created

	return 0;
}
