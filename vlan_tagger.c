#include "vlan_tagger.h"

Queue_t *in_queue;
Queue_t *out_queue;

int num_of_rules = 10;
unsigned char buffer[1522] = { 0 };
unsigned char second_buffer[1522] = { 0 };

short difine_tag_for_ip(uint32_t addr, tag_rules_t **tag_rules_obj)
{

    if (tag_rules_obj == NULL)
    {
        return -1;
    }

    if (num_of_rules == 0)
    {
        return -2;
    }

    int k = 0;

    while (k < num_of_rules &&
           (*tag_rules_obj)[k].ip_left.s_addr <= addr &&
           (*tag_rules_obj)[k].ip_right.s_addr >= addr)
    {
        k++;
    }

    if (k == num_of_rules)
    {
        return (*tag_rules_obj)[k].tag;
    }

    return 0;
}

ssize_t read_packet(void)
{
    ssize_t packet_size = 0;
    packet_size = pop(in_queue, buffer);
    if (packet_size == -1)
    {
        return 0;
    }
    if (packet_size < 46)
    {
        printL(2, 3, "The packet is the wrong size. Discarded");
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
        printL(0, 3, "The packet format is not Etnernet II");
        return -1;
    }

    if (type_field == 0x8100)
    {
        printL(0, 3, "The packet is already tagged");
        return -1;
    }

    if (type_field != 0x0800)
    {
        printL(0, 3, "The protocol of the packet is not ip");
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

int tagger(tag_rules_t **tag_rules_obj)
{
    short tag = 0;
    int k = 0;
    uint32_t addr = 0;
    ssize_t packet_size = 0;

    while (1)
    {
        if (packet_size = read_packet() == 0)
        {
            continue;
        }

        if (analyze_packet() != 0)
        {
            continue;
        }

        addr = get_packet_ip();

        switch (tag = difine_tag_for_ip(addr, tag_rules_obj))
        {
        case 0:
            continue;
            break;
        case -1:
            printL(1, 3, "No list of tagging rules");
            exit(EXIT_FAILURE);
            break;
        case -2:
            printL(1, 3, "The list of tagging rules does not contain any rules");
            exit(EXIT_FAILURE);
            break;
        }

        packet_size = packet_editor(tag, packet_size);

        if (push(out_queue, second_buffer, packet_size) != packet_size)
        {
            printL(1, 3, "Queue entry failed");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}