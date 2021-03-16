#define MSG_SIZE 8

void xor_checksum(char* dest, const char* src) {
	int i;
	for (i = 0; i < MSG_SIZE; i++)
		dest[i] ^= src[i];
}

int verify_checksum(const char* cs1, const char* cs2) {
	int i;
	for (i = 0; i < MSG_SIZE; i++)
		if (cs1[i] != cs2[i]) 
			return 0;
	return 1;
}
