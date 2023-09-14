#ifndef sniffer_header
#define sniffer_header

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <linux/if_packet.h>
#include <netinet/in.h>
#include <netinet/if_ether.h>
#include <pthread.h>
#include "queue/queue.h"
#include "logger/logger.h"
#include <errno.h>
#include <stdint.h>
#include <sys/types.h>
#include "config_parser/config_parser.h"

//Структура для отправки в потоки
typedef struct thread_data
{
    int *socket;
    int *saddr_len;
    Queue_t *sniffer_queue;
    Queue_t *sender_queue;
    const tag_rules_t *tag_rules_obj;
    struct sockaddr_ll *saddr;
} thread_data;

#define ETHERNET_FRAME_LENGTH 1522
#define THREADS_COUNT 3

void logging_programm_completion(struct thread_data params);
void pthread_init(const char *interface_name);

void *packet_sniffer(void *thread_data);

void *tagger(void *thread_data);
unsigned short packet_editor(short tag, unsigned short packet_size);
uint32_t get_packet_ip(void);
int analyze_packet(void);
ssize_t read_packet(struct thread_data params);
short define_tag_for_ip(uint32_t addr, const tag_rules_t **tag_rules_obj);

void *packet_sender(void *thread_data);

#endif
