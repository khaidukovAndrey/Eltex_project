#include <stdint.h>
#include <unistd.h>
#include <string.h>

#include "queue/queue.h"
#include "config_parser.h"

tag_rules *tag_r;
Queue_t *in_queue, *out_queue;

int num_of_rules = 10;
unsigned char buffer[1522] = { 0 };
unsigned char second_buffer[1522] = { 0 };

short difine_tag_for_ip(uint32_t addr);

int packet_editor(Queue_t *in_queue, Queue_t *out_queue);