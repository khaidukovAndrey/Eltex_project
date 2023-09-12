#ifndef H_VLAN_TAGGER
#define H_VLAN_TAGGER
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include "config_parser.h"
#include "queue/queue.h"

int tagger(void);
unsigned short packet_editor(short tag, unsigned short packet_size);
uint32_t get_packet_ip(void);
int analyze_packet(void);
unsigned short read_packet(void);
short difine_tag_for_ip(uint32_t addr);

#endif