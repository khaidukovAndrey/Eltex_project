cmake_minimum_required(VERSION 3.25)
project(vlan_tagger C)

set(CMAKE_C_STANDARD 23)

add_executable(vlan_tagger main.c vlan_tagger.c vlan_tagger.h main.c sniffer.c
        sender.c queue/queue.c queue/queue.h logger/logger.c logger/logger.h config_parser/config_parser.c
        config_parser/config_parser.h pthread_init.h pthread_init.c)

