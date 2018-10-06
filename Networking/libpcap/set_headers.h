#ifndef SET_HEADERS_H
#define SET_HEADERS_H

#include <arpa/inet.h>
#include <string.h>
#include "protocol_headers.h"

void set_en10mb_loopback(void* _header, u16 type, u8* source, u8* dest) {
	struct en10mb_header* header = (struct en10mb_header*)_header;
	memcpy(header->dhost, dest, ETHER_ADDR_LEN);
	memcpy(header->shost, source, ETHER_ADDR_LEN);
	header->type = htons(type);
}

int set_ipv4_tcp(void* _header, u16 total_length, char* source_ip, char* dest_ip) {
	struct ipv4_header* header = (struct ipv4_header*)_header;
	header->version = 0x04;
	header->ihl = 0x05;
	header->total_length = htons(total_length);
	header->dscp = 0x0;
	header->ecn = 0x0;
	header->id = htons(54321);
	header->flags_n_offset = htons(0x4000);
	header->ttl = 255;
	header->protocol = 0x06;
	header->checksum = 0x0;

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
	header->reserved = 0x0;
	header->flags = flags;
	header->window = htons(65000);
	header->checksum = 0x0;
	header->urgent_pointer = 0x0;

	if(options != NULL)
		memcpy(_header + sizeof(struct tcp_header), options, oplen);
	struct ipv4_header* ip_header = (struct ipv4_header*)((char*)_header - sizeof(struct ipv4_header));
	header->checksum = tcp_checksum(header, sizeof(struct tcp_header) + oplen + paylen, 
										ip_header->source_ip.s_addr, ip_header->dest_ip.s_addr);
}

#endif