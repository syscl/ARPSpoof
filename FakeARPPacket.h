/*
 * (c) 2018 Yating Zhou
 */
#ifndef FakeARPPacket_h
#define FakeARPPacket_h

#include "arpAttack.h"
class FakeARPPacket {
public:
    // default constructor (randomize everything)
    FakeARPPacket() {}

    // constructor with interface, router IP, victim IP as arguments
    FakeARPPacket(char* ifName, char* aRouterIP, char* aVictimIP) : interfaceNo(getInterfaceNo(ifName)), 
    routerIP(aRouterIP), victimIP(aVictimIP)
    {
        setEthIF();
    }

    // default destructor
    ~FakeARPPacket() {}

    /**
     * Send spoofed arp request packet to victim (IP, MAC)
     */
    void sendRequestPkt() const;

    /**
     * Send spoofed arp reply packet to victim (IP, MAC)
     */
    void sendReplyPkt() const;

    /**
     * Get interface number 
     */
    int getInterfaceNo() const;

    /**
     * Get interface number by name
     */
    int getInterfaceNo(char *const) const;

    /**
     * Get router IP address
     */
    char* getRouterIP() const;

    /**
     * Get victim IP address
     */
    char* getVictimIP() const;

    /**
     * Get router MAC address
     */
    uint8_t* getRouterMAC() const;

    /**
     * Get victim MAC address
     */
    uint8_t* getVictimMAC() const;

    /**
     * Set interface number
     */
    void setInterfaceNo(char*);

    /**
     * Set router IP address 
     */
    void setRouterIP(char* ip);

    /**
     * Set victim IP address
     */
    void setVictimIP(char* ip);

    /**
     * Set router MAC address
     */
    void setRouterMAC(uint8_t* mac);

    /**
     * Set victim MAC address
     */
    void setVictimMAC(uint8_t* mac);


private:
    /**
     * Generate random MAC address protects attacker from identifying
     */
    uint8_t* genFakeMACAddr(void) const;

    /**
     * Get and set ethernet interface
     * 
     * @return interface name
     */
    void setEthIF();

    /**
     * Generate and send packet based on type 
     * 
     * @param type of the packet
     * @return result
     */
    bool genAndSendPkt(int type) const;
    

protected:
    int     interfaceNo;
    char*   interfaceName;
    char*   routerIP;
    char*   victimIP;
    uint8_t routerMAC[kMACAddrSize];
    uint8_t victimMAC[kMACAddrSize];
};

#endif /* FakeARPPacket_h */ 