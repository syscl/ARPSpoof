/*
 * (c) 2018 syscl (aka Yating Zhou)
 */
#ifndef arpAttack_h
#define arpAttack_h

// don't mix linux header with netinet header together 
// this is a recipe for a disaster.
#include <netinet/ip.h>
#include <linux/if_arp.h>
#include <linux/if_link.h>
#include <linux/if_ether.h>
#include <sys/ioctl.h>
#include <net/ethernet.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstdbool>
#include <ctime>
// for memcpy
#include <cstring>
// inet_pton
#include <arpa/inet.h>

#include <netdb.h>
#include <ifaddrs.h>
#include <unistd.h>


using namespace std;

// constants defined here
constexpr int kIPv4Size    = 4;
constexpr int kMACAddrSize = 6;
constexpr int ARP_TYPE     = 0x0806;
constexpr int ARP_REQUEST  = 1;
constexpr int ARP_REPLY    = 2;
constexpr int REV_REQUEST  = 3;
constexpr int REV_REPLY    = 4;


struct _arp_t {
	/* ether header */
	uint8_t victimMAC[kMACAddrSize];
	uint8_t routerMAC[kMACAddrSize];
	uint16_t etype;
	/* end of ether_header */
	uint16_t htype;
	uint16_t ptype;
	uint8_t hlen;
	uint8_t plen;
	uint16_t opcode;
	uint8_t sender_mac[kMACAddrSize];
	uint8_t sender_ip[kIPv4Size];
	uint8_t target_mac[kMACAddrSize];
	uint8_t target_ip[kIPv4Size];
};
using arp_t = _arp_t;

/*
 * Concatenate preprocessor tokens A and B without expanding macro definitions
 * (however, if invoked from a macro, macro arguments are expanded).
 */
#define PPCAT_NX(A, B) A##B

/*
 * Concatenate preprocessor tokens A and B after macro-expanding them.
 */
#define xCAT(A, B) PPCAT_NX(A, B)
//
// define kDEBUG, true  = turn on
//              , false = turn off(default) *
//
#define kDEBUG true

#if kDEBUG
#define DEBUG_PREFIX "Debug: "
#define DBG(arg...)               \
	do                            \
	{                             \
		printf(DEBUG_PREFIX arg); \
	} while (0)
#define DBGF(arg...) \
	do               \
	{                \
		printf(arg); \
	} while (0)
#else
#define DBG(arg...) \
	do              \
	{               \
	} while (0)
#define DBGF(arg...) do {} while(0)
#endif

#endif /* arpAttack_h */