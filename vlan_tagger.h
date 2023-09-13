#ifndef H_VLAN_TAGGER
#define H_VLAN_TAGGER
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include "config_parser/config_parser.h"
#include "queue/queue.h"
#include "logger/logger.h"

int tagger(tag_rules_t** tag_rules_obj);
unsigned short packet_editor(short tag, unsigned short packet_size);
uint32_t get_packet_ip(void);
int analyze_packet(void);
ssize_t read_packet(void);
short difine_tag_for_ip(uint32_t addr, tag_rules_t** tag_rules_obj);

#endif