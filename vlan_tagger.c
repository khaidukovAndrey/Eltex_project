#include "vlan_tagger.h"

tag_rules *tag_r;
Queue_t *in_queue;
Queue_t *out_queue;

int num_of_rules = 10;
unsigned char buffer[1522] = {0};
unsigned char second_buffer[1522] = {0};

short difine_tag_for_ip(uint32_t addr)
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

unsigned short read_packet(void)
{
    unsigned short packet_size = 0;

    if (packet_size = in_queue->pkg_sizes[0] < 46)
    {
        pop(in_queue, buffer);
        return 0;
    }

    if (pop(in_queue, buffer) == 0)
    {
        return 0;
    }

    return packet_size;
}

int analyze_packet(void)
{
    short type_field = 0;
    type_field = ((0x0000 | (0xff & buffer[12])) << 8) | (0xff & buffer[13]);

    if (type_field <= 1500)
    {
        return -1;
    }

    if (type_field != 0x0800)
    {
        return -2;
    }

    return 0;
}

uint32_t get_packet_ip(void)
{
    uint32_t addr = 0;

    for (int i = 30; i < 34; i++)
    {
        addr = (addr | buffer[30]) << 8;
    }

    return addr;
}

unsigned short packet_editor(short tag, unsigned short packet_size)
{
    for (int i = 0; i < 12; i++)
    {
        second_buffer[i] = buffer[i];
    }

    second_buffer[12] = 0x81;  // first byte of TPID
    second_buffer[13] = 0x00;  // second byte of TPID
    second_buffer[14] = 0 | 0; // PCP
    second_buffer[14] <<= 1;
    second_buffer[14] = second_buffer[14] | 0; // DEI
    second_buffer[14] <<= 4;
    second_buffer[14] = second_buffer[14] | ((0x0f00 & tag) >> 8); // first 3 bits of VID
    second_buffer[15] = 0x00ff & tag;                              // VID

    for (int i = 12; i < packet_size; i++)
    {
        second_buffer[i + 4] = buffer[i];
    }

    packet_size += 4;

    return packet_size;
}

int tagger(void)
{

    short tag = 0;
    int k = 0;
    uint32_t addr = 0;
    unsigned short packet_size = 0;
    while (1)
    {
        if ((packet_size = read_packet()) == 0)
        {
            continue;
        }

        if (analyze_packet() != 0)
        {
            continue;
        }

        addr = get_packet_ip();

        switch (tag = difine_tag_for_ip(addr))
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

        packet_size = packet_editor(tag, packet_size);

        if (push(out_queue, second_buffer, packet_size) != packet_size)
        {
            perror("Error pushing packet to queue");
            exit(EXIT_FAILURE);
        }
    }
    return 0;
}