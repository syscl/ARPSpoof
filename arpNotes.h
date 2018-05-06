/*
 * (c) 2018 Yating Zhou
 * 
 * ARP, SDN & POX related template notes
 */
 
// 0.2 - header info
// 0.1 - initial version


// TODO: ARP-location-injection (victim-MAC, victim-IP). The malicious host
// injects ARP packets with the spoofed Ethernet source address of the victim to
// make the controller believe that the victim is at the same port as the attacker.
// Example: ARP-location-injection 00:00:00:00:00:04 10.0.0.4.
//
//
// DONE: ARP-map-injection (attacker-MAC, victim-IP). The malicious host
// injects ARP packets that indicate a mapping between the victim’s IP and the
// attacker’s MAC. This disrupts the IP-to-MAC mapping, and leads the controller
// to believe that the attacker has the victim’s IP address. An example of this attack
// would be ARP-map-injection 00:00:00:00:00:01 10.0.0.4.

//  This file is derived from the packet library in NOX, which was
//  developed by Nicira, Inc.

// =====================================================================
// 
//     0                   1                   2                   3
//     0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
//    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//    |          Hardware type        |             Protocol type     |
//    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//    |                  Source hardware address :::                  |
//    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//    |                  Source protocol address :::                  |
//    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//    |               Destination hardware address :::                |
//    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//    |               Destination protocol address :::                |
//    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//    |                           Data :::                            |
//    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// 
// =====================================================================

// there is generally //  places for us to check/suspect if a ARP is spoofed
// src_mac_eth = packet.src                          <--
// dst_mac_eth = packet.dst                            |
// src_ip_arp = packet.payload.protosrc                |   <--| use IP to get address from table then 
// src_mac_arp = packet.payload.hwsrc                <--   <--| compare it with hwsrc
// dst_ip_arp = packet.payload.protodst
// hosts[] table (Key, Value) = (HostIP, MAC)
if src_mac_eth != src_mac_arp:
          // 
          //  Spoofing detected
          // 
          msglog("NOTE", "Found arp spoofing attack @{0}, should be {1}".format(src_mac_eth, src_mac_arp))
          handle_spoof(src_mac_eth)
          return
        else:
          //  MAC addresses matched
          //  Check if the source ip and src MAC are matched and stored earlier;
          msglog("OK", "{0} is in table.".format(src_mac_arp))
          //  location-injection?
          if EthAddr(hosts[str(src_ip_arp)]) != src_mac_arp:
            msglog("NOTE", "Found ARP Spoofing @{0} should be {1}".format(src_mac_eth, hosts[str(src_ip_arp)]))
            handle_spoof(src_mac_eth)
            return
          else:
            //  Valid Arp Packet
            //  Check if the dest host is already there in the network
            if dst_ip_arp not in hosts.keys():
              //  Spoofing detected
              // print "Spoofing detected: Dest host ip not in table\n"
              msglog("NOTE", "{0} not exist, found ARP Spoofing @{1}".format(dst_ip_arp, src_mac_eth))
              handle_spoof(src_mac_eth)
              return
            else:
            //   Broadcast is possible also on the underlying Data Link Layer in 
            //   Ethernet networks. Frames are addressed to reach every computer on
            //   a given LAN segment if they are addressed to MAC address FF:FF:FF:FF:FF:FF. 
            //   Ethernet frames that contain IP broadcast packages are usually sent 
            //   to this address.
              if str(dst_mac_eth) == "ff:ff:ff:ff:ff:ff":
                //  Now flood the packets to all the other ports
                msglog("OK", "Flood the packets to all other ports")
              else:
                //  ARP Request should be broadcast. Some are unicast sometimes.
                msglog("OK", "Found unicast ARP packet")

// Payload: When data is sent over the Internet, each unit transmitted includes both
//  header information and the actual data being sent. 
//  The header identifies the source and destination of the packet, 
//  while the actual data is referred to as the payload.
//   Because header information, or overhead data, is only used 
//   in the transmission process, it is stripped from the packet 
//   when it reaches its destination. Therefore, the payload is the only 
//   data received by the destination system.