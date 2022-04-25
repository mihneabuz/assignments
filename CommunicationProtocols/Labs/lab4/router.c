#include "skel.h"

int interfaces[ROUTER_NUM_INTERFACES];
struct route_table_entry *rtable;
int rtable_size;

struct arp_entry *arp_table;
int arp_table_len;

/*
 Returns a pointer (eg. &rtable[i]) to the best matching route
 for the given dest_ip. Or NULL if there is no matching route.
*/
struct route_table_entry *get_best_route(__u32 dest_ip) {
	/* TODO 1: Implement the function */
	int i;
	__u32 match_mask = 0;
	struct route_table_entry *p, *match = NULL;
	for (i = 0, p = rtable; i < rtable_size; i++, p++) {
		if ((dest_ip & p->mask) == (p->prefix & p->mask)) {
			if (match == NULL || match_mask < p->mask) {
				match = p;
				match_mask = p->mask;
			}
		}
	}
	return match;
}

/*
 Returns a pointer (eg. &arp_table[i]) to the best matching ARP entry.
 for the given dest_ip or NULL if there is no matching entry.
*/
struct arp_entry *get_arp_entry(__u32 ip) {
    /* TODO 2: Implement */
	int i;
	struct arp_entry *p;
	for (i = 0, p = arp_table; i <= arp_table_len; i++, p++) {
		if (p->ip == ip) {
			return p;
		}
	}
    return NULL;
}

int main(int argc, char *argv[])
{
	msg m;
	int rc;

	init();
	rtable = malloc(sizeof(struct route_table_entry) * 100);
	arp_table = malloc(sizeof(struct  arp_entry) * 100);
	DIE(rtable == NULL, "memory");
	rtable_size = read_rtable(rtable);
	parse_arp_table();
	/* Students will write code here */

	while (1) {
		rc = get_packet(&m);
		DIE(rc < 0, "get_message");
		struct ether_header *eth_hdr = (struct ether_header *)m.payload;
		struct iphdr *ip_hdr = (struct iphdr *)(m.payload + sizeof(struct ether_header));
		
		/* TODO 3: Check the checksum */
		__u32 old_checksum = ip_hdr->check;
		ip_hdr->check = 0;
	 	__u32 new_checksum = ip_checksum(ip_hdr, sizeof(struct iphdr));

		if (old_checksum != new_checksum) {
			printf("Dropped pachet, wrong checksum!\n");
			continue;
		}

		/* TODO 4: Check TTL >= 1 */
		if (ip_hdr->ttl < 1) {
			printf("Dropped pachet, hop limit reached!\n");
			continue;
		}

		/* TODO 5: Find best matching route (using the function you wrote at TODO 1) */
		struct route_table_entry *best_match = get_best_route(ip_hdr->daddr);
		if (best_match == NULL) {
			printf("Dropped pachet, no route found!\n");
			continue;
		}

		/* TODO 6: Update TTL and recalculate the checksum */
		ip_hdr->ttl--;
	 	new_checksum = ip_checksum(ip_hdr, sizeof(struct iphdr));
		ip_hdr->check = new_checksum;

		/* TODO 7: Find matching ARP entry and update Ethernet addresses */
		struct arp_entry *arp_match = get_arp_entry(best_match->next_hop);
		if (arp_match == NULL) {
			printf("Dropped pachet, no entry in ARP table!\n");
			continue;
		}

		// update ether_header
        for (int i = 0; i < 6; i++)
			eth_hdr->ether_dhost[i] = arp_match->mac[i];
		get_interface_mac(best_match->interface, eth_hdr->ether_shost);

		/* TODO 8: Forward the pachet to best_route->interface */
		send_packet(best_match->interface, &m);
		printf("Packet routed!\n");
	}
}

