#include "rtable.h"
#include "skel.h"

#define MAX_RTABLE_ENTRY 10000

struct route_table {
	struct route_table_entry **table;	
	int *len;
};

struct route_table *rtable;

// returns number of set bits of an uint32
int count_bits(uint32_t mask) {
	int count = 0;
	while (mask) {
		mask &= (mask - 1);
		count ++;
	}
	return count;
}

// compare function for qsort
int rtable_cmp(const void *a, const void *b) {
	return (((struct route_table_entry*)a)->prefix -
			((struct route_table_entry*)b)->prefix);
}

void read_rtable(char* filename) {
	
	// allocate route table
	rtable = malloc(sizeof(struct route_table));
	rtable->table = malloc(32 * sizeof(void*));
	rtable->len = malloc(32 * sizeof(int));
	for (int i = 0; i < 32; i++) {
		rtable->table[i] = malloc(MAX_RTABLE_ENTRY * sizeof(struct route_table_entry));
		rtable->len[i] = 0;
	}

	FILE *f = fopen(filename, "rt");	

	// read each line
	while (!feof(f)) {
		uint32_t b1, b2, b3, b4;
		uint32_t prefix, next_hop, mask, interface, bucket;
		fscanf(f, "%u.%u.%u.%u ", &b1, &b2, &b3, &b4);
		prefix = b1 + (b2 << 8) + (b3 << 16) + (b4 << 24);

		fscanf(f, "%u.%u.%u.%u ", &b1, &b2, &b3, &b4);
		next_hop = b1 + (b2 << 8) + (b3 << 16) + (b4 << 24);

		fscanf(f, "%u.%u.%u.%u ", &b1, &b2, &b3, &b4);
		mask = b1 + (b2 << 8) + (b3 << 16) + (b4 << 24);

		fscanf(f, "%u\n", &interface);

		bucket = count_bits(mask) - 1;
		int len = rtable->len[bucket];

		rtable->table[bucket][len].prefix = prefix;
		rtable->table[bucket][len].next_hop = next_hop;
		rtable->table[bucket][len].mask = mask;
		rtable->table[bucket][len].interface = interface;

		rtable->len[bucket] = len + 1;

	}

	// sort all buckets
	for (int i = 0; i < 32; i++)
		qsort(rtable->table[i],
			  rtable->len[i],
			  sizeof(struct route_table_entry),
			  rtable_cmp);
}

struct route_table_entry *get_best_route(uint32_t dest_ip) {

	// binary search every bucket, in descending order
	// first prefix match we find is also longest one

	for (int bucket = 31; bucket >= 0; bucket--) {
		uint32_t ip = dest_ip & ((1ul << (bucket + 1)) - 1);	
	
		if (rtable->len[bucket] != 0) {

			// binary search
			int l, r, m;
			l = 0;
			r = rtable->len[bucket];

			while (l <= r) {
				m = l + (r - l) / 2;

				if (rtable->table[bucket][m].prefix == ip)
					return &(rtable->table[bucket][m]);
				else if (rtable->table[bucket][m].prefix > ip)
					r = m - 1;
				else
					l = m + 1;
			}
		}
	}
	return NULL;
}
