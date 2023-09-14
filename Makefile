QUEUE_WORK_NAME=queue
LIB_QUEUE_NAME=$(addprefix lib, $(QUEUE_WORK_NAME)).a
LIB_QUEUE_PATH=./queue

LOGGER_WORK_NAME=logger
LIB_LOGGER_NAME=$(addprefix lib, $(LOGGER_WORK_NAME)).a
LIB_LOGGER_PATH=./logger

PARSER_WORK_NAME=config_parser
LIB_PARSER_NAME=$(addprefix lib, $(PARSER_WORK_NAME)).a
LIB_PARSER_PATH=./config_parser

all: vlan_tagger

vlan_tagger: vlan_tagger_main.o daemon_preparation.o sniffer.o vlan_tagger.o sender.o $(LIB_QUEUE_NAME) $(LIB_LOGGER_NAME) $(LIB_PARSER_NAME)
	gcc vlan_tagger_main.o daemon_preparation.o sniffer.o vlan_tagger.o sender.o -L$(LIB_QUEUE_PATH) -l$(QUEUE_WORK_NAME) -L$(LIB_LOGGER_PATH) -l$(LOGGER_WORK_NAME) -L$(LIB_PARSER_PATH) -l$(PARSER_WORK_NAME) -o vlan_tagger

vlan_tagger_main.o: vlan_tagger_main.c vlan_tagger.h
	gcc -c vlan_tagger_main.c -o vlan_tagger_main.o

daemon_preparation.o: daemon_preparation.c vlan_tagger.h
	gcc -c daemon_preparation.c -o daemon_preparation.o -pthread

sniffer.o: sniffer.c vlan_tagger.h
	gcc -c sniffer.c -o sniffer.o

vlan_tagger.o: vlan_tagger.c vlan_tagger.h
	gcc -c vlan_tagger.c -o vlan_tagger.o

sender.o: sender.c vlan_tagger.h
	gcc -c sender.c -o sender.o

$(LIB_QUEUE_NAME):
	make -C $(LIB_QUEUE_PATH) M=$(PWD)

$(LIB_LOGGER_NAME):
	make -C $(LIB_LOGGER_PATH) M=$(PWD)

$(LIB_PARSER_NAME):
	make -C $(LIB_PARSER_PATH) M=$(PWD)


clean:
	rm -f vlan_tagger_main.o daemon_preparation.o sniffer.o vlan_tagger.o sender.o vlan_tagger
	make -C $(LIB_QUEUE_PATH) M=$(PWD) clean
	make -C $(LIB_LOGGER_PATH) M=$(PWD) clean
