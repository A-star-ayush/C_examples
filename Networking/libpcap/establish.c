#include <stdio.h>
#include <stdlib.h>
#include <pcap.h>
#include <arpa/inet.h>
#include <time.h>
#include <string.h>
#include "protocol_headers.h"
#include "checksum_utility.h"
#include "set_headers.h"

#define exit_on_error(errBuf) \
	do { fprintf(stderr, "%s\n", errBuf); exit(1); } while(0)

#define close_on_error(msg) \
	do { fprintf(stderr, "%s\n", msg); goto close; } while(0)

#define FILTER_EXP		"port 43214"	
#define SOURCE_IP 		"192.168.43.93"
#define DEST_IP 		"192.168.43.247"
#define SOURCE_PORT 	43211
#define DEST_PORT 		43214

#define CAPTURE_LEN 96

u32 seq, ack;

void readTCPInfo(u_char* userData, const struct pcap_pkthdr* header, const u_char* data){

	puts("########################################################");
	printf("time: %smicroseconds: %ld\n", asctime(localtime(&header->ts.tv_sec)), header->ts.tv_usec);
	
	const struct ipv4_header* ip_header = (const struct ipv4_header*)((u8*)data + sizeof(struct en10mb_header));
	if(ip_header->version != 4)
		puts("not an ipv4 packet.");
	else {
		puts("IPv4 packet");
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
	
		seq = ntohl(tcp_header->sequence);
		ack = ntohl(tcp_header->acknowledge);

		// if(tcp_header->flags & 0x10) pcap_breakloop((void*)userData);
	}
}

int main(int argc, char *argv[]){

	int rt;
	char errBuf[PCAP_ERRBUF_SIZE];
	char* interface = (argc < 2) ? "any" : argv[1];

	pcap_t* handle = pcap_create(interface, errBuf);
	if(handle == NULL) exit_on_error(errBuf);

	rt = pcap_set_snaplen(handle, CAPTURE_LEN);
	if(rt!=0)
		fprintf(stderr, "There was a problem with pcap_set_snaplen\n");
	
	rt = pcap_activate(handle);
	if(rt!=0) close_on_error("error occured activating device");
	
	struct bpf_program program;
	rt = pcap_compile(handle, &program, FILTER_EXP, 1, PCAP_NETMASK_UNKNOWN);
	if(rt == -1) close_on_error(pcap_geterr(handle));
	rt = pcap_setfilter(handle, &program);
	if(rt == -1) close_on_error(pcap_geterr(handle));
	pcap_freecode(&program);

	printf("Link layer header type: %s\n", pcap_datalink_val_to_name(pcap_datalink(handle)));
	
	size_t ll_header_sz = sizeof(struct en10mb_header);
	size_t ip_header_sz = sizeof(struct ipv4_header);
	size_t tcp_header_sz = sizeof(struct tcp_header);
	
	uint8_t source[] = {  0xfc, 0xf8, 0xae, 0x04, 0xee, 0x67 };
	uint8_t dest[] = { 0x08, 0x00, 0x27, 0x0f, 0x20, 0x64 };
	
/*	pcap_loop(handle, 3, readTCPInfo, NULL);

	{
		char data[] = "hi. My name is Ayush";
		size_t total_length = ll_header_sz + ip_header_sz + tcp_header_sz + sizeof(data);
		char packet[total_length];

		set_en10mb_loopback(packet, 0x0800, source, dest);
		rt = set_ipv4_tcp(packet + ll_header_sz, total_length - ll_header_sz, SOURCE_IP, DEST_IP);
		if(rt == -1) close_on_error("Invalid source ipv4 addresses");
		if(rt == -2) close_on_error("Invalid destination ipv4 addresses");
		memcpy(packet + total_length - sizeof(data), data, sizeof(data));
		set_tcp(packet + ll_header_sz + ip_header_sz, SOURCE_PORT, DEST_PORT, seq, ack, 1<<4 | 1<<3, NULL, 0, sizeof(data));

		rt= pcap_inject(handle, packet, total_length);
		if(rt == -1) close_on_error(pcap_geterr(handle));
		else printf("Sent %u bytes\n", rt);

	}
	
	pcap_loop(handle, 1, readTCPInfo, NULL);
*/
	// Send SYN
	{
		size_t total_length = ll_header_sz + ip_header_sz + tcp_header_sz;
		char packet[total_length];

		set_en10mb_loopback(packet, 0x0800, source, dest);
		rt = set_ipv4_tcp(packet + ll_header_sz, total_length - ll_header_sz, SOURCE_IP, DEST_IP);
		if(rt == -1) close_on_error("Invalid source ipv4 addresses");
		if(rt == -2) close_on_error("Invalid destination ipv4 addresses");
		set_tcp(packet + ll_header_sz + ip_header_sz, SOURCE_PORT, DEST_PORT, 10000, 0, 0x02, 
					NULL, 0, 0);

		rt= pcap_inject(handle, packet, total_length);
		if(rt == -1) close_on_error(pcap_geterr(handle));
		else printf("Sent %u bytes\n", rt);
	}

	// Read SYN + ACK
	rt = pcap_loop(handle, -1, readTCPInfo, (void*)(handle));
	if(rt == -1) close_on_error(pcap_geterr(handle));
	puts("########################################################");


	// Send ACK
	{
		size_t total_length = ll_header_sz + ip_header_sz + tcp_header_sz;
		char packet[total_length];

		set_en10mb_loopback(packet, 0x0800, source, dest);
		rt = set_ipv4_tcp(packet + ll_header_sz, total_length - ll_header_sz, SOURCE_IP, DEST_IP);
		if(rt == -1) close_on_error("Invalid source ipv4 addresses");
		if(rt == -2) close_on_error("Invalid destination ipv4 addresses");
		set_tcp(packet + ll_header_sz + ip_header_sz, SOURCE_PORT, DEST_PORT, ack, seq + 1, 1<<4, NULL, 0, 0);

		rt= pcap_inject(handle, packet, total_length);
		if(rt == -1) close_on_error(pcap_geterr(handle));
		else printf("Sent %u bytes\n", rt);		
	}

	{
		u32 t_seq = seq;
		seq = ack;
		ack = t_seq + 1;
	}

	// Send "hi..."
	{
	
		char data[] = "hi. My name is Ayush";
		size_t total_length = ll_header_sz + ip_header_sz + tcp_header_sz + sizeof(data);
		char packet[total_length];

		set_en10mb_loopback(packet, 0x0800, source, dest);
		rt = set_ipv4_tcp(packet + ll_header_sz, total_length - ll_header_sz, SOURCE_IP, DEST_IP);
		if(rt == -1) close_on_error("Invalid source ipv4 addresses");
		if(rt == -2) close_on_error("Invalid destination ipv4 addresses");
		memcpy(packet + total_length - sizeof(data), data, sizeof(data));
		set_tcp(packet + ll_header_sz + ip_header_sz, SOURCE_PORT, DEST_PORT, seq, ack, 1<<4 | 1<<3, NULL, 0, sizeof(data));

		rt= pcap_inject(handle, packet, total_length);
		if(rt == -1) close_on_error(pcap_geterr(handle));
		else printf("Sent %u bytes\n", rt);		
	}

	rt = pcap_loop(handle, -1, readTCPInfo, (void*)(handle));
	if(rt == -1) close_on_error(pcap_geterr(handle));
	puts("########################################################");


	struct pcap_stat ps;
	rt = pcap_stats(handle, &ps);
	if(rt == -1)
		close_on_error(pcap_geterr(handle));
	else{
		puts("////////////////////////////////////////////////");
		printf("Number of packets received: %u\n", ps.ps_recv);
		printf("Number of packets dropped due to no room: %u\n", ps.ps_drop);
		printf("Number of packets dropped by the interface or dirver: %u\n", ps.ps_ifdrop);
		puts("////////////////////////////////////////////////");
	}

	getchar();

	close:
	pcap_close(handle);

	return rt;
}