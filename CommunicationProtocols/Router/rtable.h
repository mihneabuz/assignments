struct route_table_entry {
	unsigned int prefix;
	unsigned int next_hop;
	unsigned int mask;
	unsigned int interface;
} __attribute__((packed));

void read_rtable(char* filename);

struct route_table_entry *get_best_route(unsigned int dest_ip);
