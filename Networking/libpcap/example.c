#include <stdio.h>
#include <stdlib.h>
#include <pcap.h>
#include <arpa/inet.h>
#include <time.h>
#include <string.h>
#include "protocol_headers.h"
#include "checksum_utility.h"

// link with -lpcap
// might have to run it with sudo
// define endianess by: -D ENDIANESS=LITTLE_ENDIAN/BIG_ENDIAN .. little-endian will be used by default

#define exit_on_error(errBuf) \
	do { fprintf(stderr, "%s\n", errBuf); exit(1); } while(0)

#define close_on_error(msg) \
	do { fprintf(stderr, "%s\n", msg); goto close; } while(0)

#define OUTPUT_FILE 	"capture.example"
#define FILTER_EXP		"port 43210"	
#define SOURCE_IP 		"127.0.0.1"
#define DEST_IP 		"127.0.0.1"
#define SOURCE_PORT 	43211
#define DEST_PORT 		43210

#define CAPTURE_LEN 96

#define print_mac_address(buf) \
	printf("%02x:%02x:%02x:%02x:%02x:%02x\n", buf[0], buf[1], buf[2], buf[3], buf[4], buf[5]);

void readPacket(u_char* userData, const struct pcap_pkthdr* header, const u_char* data){

	// For more on the details and accuracy of the timestamps refer to man 7 pcap-tstamp
	puts("########################################################");
	printf("time: %smicroseconds: %ld\n", asctime(localtime(&header->ts.tv_sec)), header->ts.tv_usec);
	printf("caplen: %d\n", header->caplen);
	printf("len: %d\n", header->len);

	// header and data are not to be freed by the callback routine, and are not gauranteed to be valid after
	// the callback routine return, thus copy the data you are interested to use later on.

	// dumping out the packets to a savefile
	pcap_dump(userData, header, data);

	// The bytes of packet begin with a link layer header
	const struct en10mb_header* ll_header = (const struct en10mb_header*)data;
	printf("Destination MAC addresses: ");
	print_mac_address(ll_header->dhost);
	printf("Source MAC addresses: ");
	print_mac_address(ll_header->shost);
	printf("Type: %s\n", ntohs(ll_header->type) == 0x0800 ? "IP packet" : "Not an IP packet");

	const struct ipv4_header* ip_header = (const struct ipv4_header*)((u8*)data + sizeof(struct en10mb_header));
	if(ip_header->version != 4)
		puts("not an ipv4 packet.");
	else {
		puts("IPv4 packet");
		printf("Header Length: %d\n", ip_header->ihl*4);
		printf("Total Length: %d\n", ntohs(ip_header->total_length));

		const struct tcp_header* tcp_header = (const struct tcp_header*)((u8*)ip_header + (ip_header->ihl * 4));

		printf("Source %s : %d\n", inet_ntoa(ip_header->source_ip), ntohs(tcp_header->source_port));
		printf("Destination %s : %d\n", inet_ntoa(ip_header->dest_ip), ntohs(tcp_header->dest_port));
		printf("TCP Flags:\n");
		if(tcp_header->flags & 0x01) printf("FIN ");
		if(tcp_header->flags & 0x02) printf("SYN ");
		if(tcp_header->flags & 0x04) printf("RST ");
		if(tcp_header->flags & 0x08) printf("PSH ");
		if(tcp_header->flags & 0x10) printf("ACK ");
		printf("[ %u , %u ]", ntohl(tcp_header->sequence), ntohl(tcp_header->acknowledge));
		puts("");
	}
}

void set_en10mb_loopback(void* _header, u16 type) {
	struct en10mb_header* header = (struct en10mb_header*)_header;
	header->type = htons(type);
}

int set_ipv4_tcp(void* _header, u16 total_length, char* source_ip, char* dest_ip) {
	struct ipv4_header* header = (struct ipv4_header*)_header;
	header->version = 0x04;
	header->ihl = 0x05;
	header->dscp = 0x04;
	header->total_length = htons(total_length);
	header->id = htons(10000);
	header->flags_n_offset = htons(0x4000);
	header->ttl = 64;
	header->protocol = 0x06;

	int rt = inet_aton(source_ip, &header->source_ip);
	if(rt == 0) return -1;

	rt = inet_aton(dest_ip, &header->dest_ip);
	if(rt == 0) return -2;
	
	header->checksum = ip_checksum(header, header->ihl*4);

	return 0;
}

void set_tcp(void* _header, u16 source_port, u16 dest_port, u32 seq, u32 ack, u8 flags, u8* options, u8 oplen, u16 paylen) {
	struct tcp_header* header = (struct tcp_header*)(_header);
	header->source_port = htons(source_port);
	header->dest_port = htons(dest_port);
	header->sequence = htonl(seq);
	header->acknowledge = htonl(ack);
	header->offset = 0x05 + (oplen>>2);
	header->flags = flags;
	header->window = htons(65000);
	memcpy(_header + sizeof(struct tcp_header), options, oplen);
	struct ipv4_header* ip_header = (struct ipv4_header*)((char*)_header - sizeof(struct ipv4_header));
	header->checksum = tcp_checksum(header, sizeof(struct tcp_header) + oplen + paylen, 
										ip_header->source_ip.s_addr, ip_header->dest_ip.s_addr);
}


int main(int argc, char *argv[]){

	printf("libpcap version: %s\n", pcap_lib_version());

	int rt;
	char errBuf[PCAP_ERRBUF_SIZE];
	char* interface = (argc < 2) ? "any" : argv[1];  // on linux specifying "any" or NULL can be used to capture all interfaces

	// create a live capture
	pcap_t* handle = pcap_create(interface, errBuf);
	if(handle == NULL) 
		exit_on_error(errBuf);

	// set options before activating
	pcap_set_snaplen(handle, CAPTURE_LEN);
	
	// activate the handle (only currently relevant return value cases were considered)
	rt = pcap_activate(handle);
	switch(rt){
		case 0: break;
		case PCAP_ERROR_ACTIVATED: close_on_error("The handle has already been activated.");
		case PCAP_ERROR_IFACE_NOT_UP: close_on_error("The interface is not up");
		case PCAP_ERROR_PERM_DENIED:
		case PCAP_ERROR_NO_SUCH_DEVICE:
		case PCAP_ERROR:
		case PCAP_WARNING: close_on_error(pcap_geterr(handle));
		default: close_on_error("Some other error occured");
	}

	// Setting a link layer header type (to be done after activation but before reading a packet)
	rt = pcap_set_datalink(handle, DLT_LINUX_SLL);  // Raw IP; packet begins with an IPv4 or IPv6 header
	if(rt == -1) {
		fprintf(stderr, "Couldn't set DLT_LINUX_SLL.\n");
		int* dlts;
		int n = pcap_list_datalinks(handle, &dlts);
		if(n < 1)
			fprintf(stderr, "%s\n", pcap_geterr(handle));
		else{
			puts("The list of supported link-layer header types is as follows:");
			int i;
			for(i=0;i<n;++i)
				printf("%s\n", pcap_datalink_val_to_name(dlts[i]));
			pcap_free_datalinks(dlts);
			puts("");
		}
	}

	// Applying a filter
	struct bpf_program program;  // compile creates a program and the program is then associated with a filter
	rt = pcap_compile(handle, &program, FILTER_EXP, 1, PCAP_NETMASK_UNKNOWN);  // checkout saved pages for filter expressions
	if(rt == -1) close_on_error(pcap_geterr(handle));
	// the last arg of netmask is needed only for broadcast addresses (pcap_lookupnet can be used to find the mask)
	rt = pcap_setfilter(handle, &program);
	if(rt == -1) close_on_error(pcap_geterr(handle));
	pcap_freecode(&program);


	/*// Opening a handle for writing captured packets
	pcap_dumper_t* outFile = pcap_dump_open(handle, OUTPUT_FILE);  // specify second arg as "-" for stdout

	if(outFile == NULL)
		close_on_error("couldn't open the dump file");


	// Reading packets (sample read of 5 packets; to keep reading by registering a function handler use pcap_dispatch/loop)
	rt = pcap_loop(handle, 5, readPacket, (u_char*)outFile);  // use -1 in arg 2 to keep reading packets infinitely
	// The calls to read are by default "blocking". Can be made otherwise by using pcap_setnonblock and more.
	// pcap_loop processes packets until an equivalent of arg 2 packets have been read, EOF for a savefile is reached 
	// or an error occurs. If you want to read bufferful of packets use pcap_dispatch instead.
	if(rt == -1) 
		fprintf(stderr, "%s\n", pcap_geterr(handle));  // -1: error occured while reading the packet
	
	
	pcap_dump_close(outFile);
*/
	
	// Let's try injecting packets (the data should include the link-layer header as well)

	/*size_t ll_header_sz = sizeof(struct en10mb_header);
	size_t ip_header_sz = sizeof(struct ipv4_header);
	size_t tcp_header_sz = sizeof(struct tcp_header);
	
	char tcp_options[4] = {0x02, 0x04, 0xff, 0xd7};

	size_t total_length = ll_header_sz + ip_header_sz + tcp_header_sz + sizeof(tcp_options);

	char* packet = calloc(1, total_length);  // calloc to initialize the fields with zero

	set_en10mb_loopback(packet, 0x0800);

	rt = set_ipv4_tcp(packet + ll_header_sz, total_length, SOURCE_IP, DEST_IP);
	if(rt == -1) close_on_error("Invalid source ipv4 addresses");
	if(rt == -2) close_on_error("Invalid destination ipv4 addresses");

	set_tcp(packet + ll_header_sz + ip_header_sz, SOURCE_PORT, DEST_PORT, 10000, 0, 0x02, tcp_options, sizeof(tcp_options), 0);

	rt= pcap_inject(handle, packet, total_length);
	if(rt == -1)
		fprintf(stderr, "%s\n", pcap_geterr(handle));
	else
		printf("Sent %u bytes of data\n", rt);

	free(packet);
	*/

	// close the handle to release resources
	close:
	pcap_close(handle);

	return rt;
}