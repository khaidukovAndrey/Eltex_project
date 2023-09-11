#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include "config_parser.h"
#include "queue/queue.h"

tag_rules *tag_r;
Queue_t *in_queue, *out_queue;

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

int packet_editor()
{
    init(in_queue);
    init(out_queue);

    unsigned short tag = 0;
    int k = 0;
    uint32_t addr=0;
    while (1)
    {
        if (pop(&in_queue, &buffer) == 0)
        {
            continue;
        }
        
        if ((((0x0000 | (0xff & buffer[12])) << 8) | (0xff & buffer[13])) <= 1500)
        {
            continue;
        }
        addr=0;
        addr=addr | buffer[30];
        addr = addr<<8;
        addr=addr | buffer[31];
        addr = addr<<8;
        addr=addr | buffer[32];
        addr = addr<<8;
        addr=addr | buffer[33];
        addr = addr<<8;

        switch(tag = difine_tag_for_ip(addr))
        {
            case 0:
                continue;
            break;
            case -1:
                perror("No rule list");
                exit(EXIT_FAILURE);
            break;
            case -2:
                perror("No rules");
                exit(EXIT_FAILURE);
            break;
        }

        while (k != 12)
        {
            second_buffer[k] = buffer[k];
            k++;
        }

        second_buffer[k] = 0x81;      // first byte of TPID
        second_buffer[k + 1] = 0x00;  // second byte of TPID
        second_buffer[k + 2] = 0 | 0; // PCP
        second_buffer[k + 2] <<= 1;
        second_buffer[k + 2] = second_buffer[k + 2] | 0; // DEI
        second_buffer[k + 2] <<= 4;
        second_buffer[k + 2] = second_buffer[k + 2] | ((0x0f00 & tag)>>8); // first 3 bits of VID
        second_buffer[k + 3] = 0x00ff & tag;                         // VID

        while (buffer[k] != "\0" && k <1522)
        {
            second_buffer[k + 4] = buffer[k];
            k++;
        }

        push(out_queue, second_buffer, 1522);
    }
    return 0;
}