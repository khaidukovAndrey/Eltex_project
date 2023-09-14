QUEUE_WORK_NAME=queue
LIB_QUEUE_NAME=$(addprefix lib, $(QUEUE_WORK_NAME)).a
LIB_QUEUE_PATH=./queue

<<<<<<< Makefile

LOGGER_WORK_NAME=logger
LIB_LOGGER_NAME=$(addprefix lib, $(LOGGER_WORK_NAME)).a
LIB_LOGGER_PATH=./logger

all: vlan_sniffer
=======
all: vlan_tagger
>>>>>>> Makefile

vlan_tagger: vlan_tagger.o simple_sniffer.o vlan_tagger_main.o libqueue.a
	gcc vlan_tagger.o vlan_tagger_main.o simple_sniffer.o -L$(LIB_QUEUE_PATH) -lqueue -o vlan_tagger

vlan_tagger.o: vlan_tagger.c vlan_tagger.h config_parser.h
	gcc -c vlan_tagger.c -o vlan_tagger.o

vlan_tagger_main.o: vlan_tagger_main.c sniffer_header.h vlan_tagger.h
	gcc -c vlan_tagger_main.c -o vlan_tagger_main.o

simple_sniffer.o: simple_sniffer.c sniffer_header.h	
	gcc -c simple_sniffer.c -o simple_sniffer.o

$(LIB_QUEUE_NAME):
	make -C $(LIB_QUEUE_PATH) M=$(PWD)

$(LIB_LOGGER_NAME):
	make -C $(LIB_LOGGER_PATH) M=$(PWD)

clean:
<<<<<<< Makefile
	rm -f vlan_tagger_main.o simple_sniffer.o vlan_sniffer vlan_tagger.o vlan_tagger
	make -C $(LIB_QUEUE_PATH) M=$(PWD) clean
	make -C $(LIB_LOGGER_PATH) M=$(PWD) clean
	
=======
	rm -f vlan_tagger_main.o simple_sniffer.o vlan_tagger.o vlan_tagger; make -C $(LIB_QUEUE_PATH) M=$(PWD) clean
>>>>>>> Makefile
