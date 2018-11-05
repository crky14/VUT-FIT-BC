/**
 * Project: 
 * Class: ISA
 * Year: 2018/2019
 * University: VUTBR FIT
 * Login: xcrkon00
 * Author: Jakub Crkoň
 */

#ifndef STRUCTURES_H 
#define STRUCTURES_H

#include <netinet/in.h>

/* Structures */

/* Ethernet addresses are 6 bytes */
#define ETHER_ADDR_LEN	6

#define SIZE_ETHERNET 14

/* Ethernet header */
struct sniff_ethernet {
	unsigned char ether_dhost[ETHER_ADDR_LEN]; /* Destination host address */
	unsigned char ether_shost[ETHER_ADDR_LEN]; /* Source host address */
	unsigned short ether_type; /* IP? ARP? RARP? etc */
};

/* IP header */
struct sniff_ip {
	unsigned char ip_vhl;		/* version << 4 | header length >> 2 */
	unsigned char ip_tos;		/* type of service */
	unsigned short ip_len;		/* total length */
	unsigned short ip_id;		/* identification */
	unsigned short ip_off;		/* fragment offset field */
    #define IP_RF 0x8000		/* reserved fragment flag */
    #define IP_DF 0x4000		/* dont fragment flag */
    #define IP_MF 0x2000		/* more fragments flag */
    #define IP_OFFMASK 0x1fff	/* mask for fragmenting bits */
	unsigned char ip_ttl;		/* time to live */
	unsigned char ip_p;			/* protocol */
	unsigned short ip_sum;		/* checksum */
	struct in_addr ip_src,ip_dst; /* source and dest address */
};

#define IP_HL(ip)		(((ip)->ip_vhl) & 0x0f)
#define IP_V(ip)		(((ip)->ip_vhl) >> 4)

/* TCP header */
struct sniff_tcp {
	unsigned short th_sport;	/* source port */
	unsigned short th_dport;	/* destination port */
	unsigned int th_seq;		/* sequence number */
	unsigned int th_ack;		/* acknowledgement number */
	unsigned char th_offx2;		/* data offset, rsvd */
	unsigned char th_flags;
	unsigned short th_win;		/* window */
	unsigned short th_sum;		/* checksum */
	unsigned short th_urp;		/* urgent pointer */
};

struct sniff_udp {
	unsigned short th_sport;	/* source port */
	unsigned short th_dport;	/* destination port */
	unsigned short udp_len;	/* UPD length */
	unsigned short th_sum;	/* checksum */
};

struct sniff_dns {
    unsigned short th_id;
};

#endif