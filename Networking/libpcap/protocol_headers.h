#ifndef PROTOCOL_HEADERS_H
#define PROTOCOL_HEADERS_H

#include <stdint.h>
#include <netinet/in.h>

#define L_ENDIAN 0
#define B_ENDIAN 1

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

#define ETHER_ADDR_LEN	6

struct en10mb_header {
		u8 dhost[ETHER_ADDR_LEN];
		u8 shost[ETHER_ADDR_LEN];
		u16 type;
} __attribute__ ((__packed__));

struct linux_sll_header {
	u16 packet_type;
	u16 arphrd_type;
	u16 ll_addr_length;
	u8 ll_addr[8];
	u16 protocol_type;
} __attribute__ ((__packed__));

struct ipv4_header {
	struct{
		#if ENDIANESS && ENDIANESS==B_ENDIAN
			u8 version:4;
			u8 ihl:4;
		#else
			u8 ihl: 4;
			u8 version: 4;
		#endif
	};

	struct {
		#if ENDIANESS && ENDIANESS==B_ENDIAN
			u8 dscp: 6;
			u8 ecn: 2;
		#else
			u8 ecn: 2;
			u8 dscp: 6;
		#endif
	};

	u16 total_length;
	u16 id;

	u16 flags_n_offset;
	u8 ttl;
	u8 protocol;
	u16 checksum;
	struct in_addr source_ip;
	struct in_addr dest_ip;
} __attribute__ ((__packed__));

struct tcp_header {
	u16 source_port;
	u16 dest_port;
	u32 sequence;
	u32 acknowledge;

	struct{
		#if ENDIANESS && ENDIANESS==B_ENDIAN
			u8 offset: 4;
			u8 reserved: 4;
		#else 
			u8 reserved: 4;
			u8 offset: 4;
		#endif
	};

	u8 flags;
	u16 window;
	u16 checksum;
	u16 urgent_pointer;
} __attribute__ ((__packed__));

#endif