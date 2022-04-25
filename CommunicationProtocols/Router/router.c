#include "skel.h"
#include "queue.h"
#include "rtable.h"
#include "arptable.h"

// array of router IP adresses
uint32_t interface_map[ROUTER_NUM_INTERFACES];

// waiting queue
queue waiting;

void forward_packet(packet *m) {
	
	// get headers
	struct ether_header *eth_hdr = (struct ether_header *) m->payload;
	struct iphdr *ip_hdr = (struct iphdr *)(m->payload + sizeof(struct ether_header));
	
	// find best route
	struct route_table_entry *best_match = get_best_route(ip_hdr->daddr);
	if (best_match == NULL) {

		// if no route found, send icmp error
		send_icmp_error(ip_hdr->saddr,
						interface_map[m->interface],
						eth_hdr->ether_dhost,
						eth_hdr->ether_shost,
						ICMP_DEST_UNREACH,
						0,
						m->interface);
		return;
	}

	// find arp entry for next_hop
	struct arp_entry *arp_match = get_arp_entry(best_match->next_hop);
	if (arp_match == NULL) {

		// build ether_header
		struct ether_header *eth_req = malloc(sizeof(struct ether_header));
		get_interface_mac(best_match->interface, eth_req->ether_shost);
		memset(eth_req->ether_dhost, 0xff, 6);
		eth_req->ether_type = htons(ETHERTYPE_ARP);

		// if no arp entry, send arp request and push m in waiting queue
		send_arp(best_match->next_hop,
				 interface_map[best_match->interface],
				 eth_req,
				 best_match->interface,
				 htons(ARPOP_REQUEST));

		free(eth_req);

		packet *aux = malloc(sizeof(packet));
		memcpy(aux, m, sizeof(packet));
		queue_enq(waiting, aux);
		
		return;
	} 

	// update ether_header
	memcpy(eth_hdr->ether_dhost, arp_match->mac, 6);
	get_interface_mac(best_match->interface, eth_hdr->ether_shost);
	
	// forward packet
	send_packet(best_match->interface, m);
}

int main(int argc, char *argv[])
{
	// init tables
	read_rtable(argv[1]);
	init_arptable();

	packet m;
	int rc;

	init(argc - 2, argv + 2);
	
	// make a map of all local ip's
	for (int i = 0; i < ROUTER_NUM_INTERFACES; i++)
		interface_map[i] = inet_addr(get_interface_ip(i));

	waiting = queue_create();

	// main loop
	while (1) {
		rc = get_packet(&m);
		DIE(rc < 0, "get_message");

		struct ether_header *eth_hdr = (struct ether_header *) m.payload;

		struct arp_header* arp_hdr = parse_arp(m.payload);
		if (arp_hdr) {
			// got an ARP packet
			
			if (ntohs(arp_hdr->op) == ARPOP_REQUEST) {
			
				// find target interface
			 	int interface = -1;
				for (int i = 0; i < ROUTER_NUM_INTERFACES; i++)
					if (arp_hdr->tpa == interface_map[i]) {
						interface = i;
						break;
					}

				// if it's an address of this router, send arp reply with mac address 
				if (interface > -1) {

					// build ether header
					struct ether_header *eth_reply = malloc(sizeof(struct ether_header));
					memcpy(eth_reply->ether_dhost, arp_hdr->sha, 6);
					get_interface_mac(interface, eth_reply->ether_shost);
					eth_reply->ether_type = htons(ETHERTYPE_ARP);

					// send reply
					send_arp(arp_hdr->spa,
							 arp_hdr->tpa,
							 eth_reply,
							 m.interface,
							 htons(ARPOP_REPLY));

					free(eth_reply);
				}
			}
			else if (ntohs(arp_hdr->op) == ARPOP_REPLY) {

				// add entry to ARP table
				add_arp_entry(arp_hdr->spa, arp_hdr->sha);
		
				// try sending packets from waiting queue
				while (!queue_empty(waiting)) {
					packet *old = queue_deq(waiting);
					forward_packet(old);
					free(old);
				}
			}

		}
		else {
			struct iphdr *ip_hdr = (struct iphdr *)(m.payload + sizeof(struct ether_header));

			// check if packet is for router
			int for_router = 0;
			for (int i = 0; i < ROUTER_NUM_INTERFACES && !for_router; i++)
				if (ip_hdr->daddr == interface_map[i])
					for_router = 1;

			// check if packet is ICMP echo for router
			struct icmphdr *icmp_hdr = parse_icmp(m.payload);
			if (for_router && icmp_hdr && icmp_hdr->type == ICMP_ECHO) {

				// send echo reply to echo request
				send_icmp(ip_hdr->saddr,
						  ip_hdr->daddr,
						  eth_hdr->ether_dhost,
						  eth_hdr->ether_shost,
						  ICMP_ECHOREPLY,
						  0,
						  m.interface,
						  icmp_hdr->un.echo.id,
						  icmp_hdr->un.echo.sequence);
			}

			if (!for_router) {
				// Prepare IP packet for Forwarding
					
				// check TTL
				if (ip_hdr->ttl <= 1) {

					send_icmp_error(ip_hdr->saddr,
									interface_map[m.interface],
									eth_hdr->ether_dhost,
									eth_hdr->ether_shost,
									ICMP_TIME_EXCEEDED,
									0,
									m.interface);

					continue;
				}
				
				// validate checksum
				uint16_t old_check = ip_hdr->check;
				ip_hdr->check = 0;
				if (ip_checksum(ip_hdr, sizeof(struct iphdr)) != old_check)
					continue;
				
				// decresease TTL and update checksum
				ip_hdr->ttl--;
				ip_hdr->check = 0;
				ip_hdr->check = ip_checksum(ip_hdr, sizeof(struct iphdr));

				// forward the packet to next hop
				forward_packet(&m);
			}
		}
	}
}
