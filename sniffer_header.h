#ifndef sniffer_header
#define sniffer_header

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <linux/if_packet.h>
#include <netinet/in.h>
#include <netinet/if_ether.h>

#define ETHERNET_FRAME_LENGTH 1522

void packet_sniffer(const char *interface_name);

#endif
