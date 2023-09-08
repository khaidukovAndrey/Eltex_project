all: vlan_sniffer

vlan_sniffer: vlan_tagger_main.o simple_sniffer.o
	gcc vlan_tagger_main.o simple_sniffer.o -o vlan_sniffer

vlan_tagger_main.o: vlan_tagger_main.c sniffer_header.h
	gcc -c vlan_tagger_main.c -o vlan_tagger_main.o

simple_sniffer.o: simple_sniffer.c sniffer_header.h
	gcc -c simple_sniffer.c -o simple_sniffer.o

clean:
	rm -f vlan_tagger_main.o simple_sniffer.o vlan_sniffer
