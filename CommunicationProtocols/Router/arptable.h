#include <bits/stdint-uintn.h>

struct arp_entry {
	uint32_t ip;
	uint8_t mac[6];
} __attribute__((packed));

void init_arptable();

void add_arp_entry(uint32_t ip, uint8_t mac[6]);

struct arp_entry *get_arp_entry(uint32_t ip);
