#include "arptable.h"
#include "skel.h"

#define MAX_ARP_TABLE_LEN 100

struct arp_entry *arp_table;
int arp_table_len;

void init_arptable() {
	arp_table = malloc(MAX_ARP_TABLE_LEN * sizeof(struct arp_entry));
	arp_table_len = 0;	
}

void add_arp_entry(uint32_t ip, uint8_t mac[6]) {
	for (int i = 0; i < arp_table_len; i++)
		if (arp_table[i].ip == ip) {
			memcpy(arp_table[i].mac, mac, 6);
			return;
		}
	arp_table[arp_table_len].ip = ip;
	memcpy(arp_table[arp_table_len].mac, mac, 6);
	arp_table_len += 1;
}

struct arp_entry *get_arp_entry(uint32_t ip) {
	int i;
	struct arp_entry *p;
	for (i = 0, p = arp_table; i <= arp_table_len; i++, p++) {
		if (p->ip == ip) {
			return p;
		}
	}
    return NULL;
}
