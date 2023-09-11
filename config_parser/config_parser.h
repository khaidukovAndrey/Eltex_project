#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

typedef struct tag_rules
{
    struct in_addr ip_left;
    struct in_addr ip_right;
    int tag;
} tag_rules;

int tag_rules_init(tag_rules **tag_rules_obj, int size);
void tag_rules_clear(tag_rules **tag_rules_obj);
int tag_rules_check_collisions(tag_rules *tag_rules_obj, int size);

int config_file_check(void);
int config_file_read(tag_rules *tag_rules_obj);

int ip_converting(struct in_addr *in_addr_obj, const char *ip_str);
int tag_converting(int *tag_int, const char *tag_str);
int ip_comparison(struct in_addr ip_1, struct in_addr ip_2);
int ip_str_to_int(char *ip_str, int *ip_int);
int char_count(char *str, char symbol);
