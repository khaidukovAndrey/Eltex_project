#ifndef H_VLAN_TAGGER
#define H_VLAN_TAGGER
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include "config_parser.h"
#include "queue/queue.h"

short difine_tag_for_ip(uint32_t addr);
int packet_editor(Queue_t *in, Queue_t *out);

#endif