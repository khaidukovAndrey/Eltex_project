#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include "config_parser.h"
#include "queue/queue.h"

tag_rules *tag_r;
Queue *in_queue, *out_queue;

int num_of_rules = 10;
unsigned char buffer[1522] = {0};
unsigned char second_buffer[1522] = {0};

unsigned short difine_tag_for_ip(uint32_t addr)
{
    if (tag_r == NULL)
    {
        return -1;
    }

    if (num_of_rules == 0)
    {
        return -2;
    }

    int k = 0;

    while (k < num_of_rules && tag_r[k].left_board <= addr && tag_r[k].right_board >= addr)
    {
        k++;
    }

    if (k == num_of_rules)
    {
        return tag_r[k].tag;
    }

    return 0;
}