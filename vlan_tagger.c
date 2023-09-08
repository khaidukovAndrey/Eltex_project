#include <stdint.h>
#include <unistd.h>
#include "config_parser.h"

tag_rules *tagRules;

int numOfRules = 10;

int difine_tag_for_ip(uint32_t addr)
{
    if (tagRules == NULL)
    {
        return -1;
    }
    if (numOfRules == 0)
    {
        return -2;
    }
    int k = 0;
    while (k < numOfRules && tagRules[k].left_board <= addr && tagRules[k].right_board >= addr)
    {
        k++;
    }
    if (k == numOfRules)
    {
        return tagRules[k].tag;
    }
    return 0;
}