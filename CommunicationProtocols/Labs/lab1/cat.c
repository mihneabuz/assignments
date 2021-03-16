#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFER_SIZE 512

void fatal(char* error) {
	perror(error);
	exit(0);
}

int main(int argc, char** argv) {
	
	int src, dest = 1; // stdout = 1
	
	int i, read_bytes, wrote_bytes;
	char buffer[BUFFER_SIZE];

	for (i = 1; i < argc; i++) {
		src = open(argv[i], O_RDONLY);
		if (src < 0)
			fatal("Cannot open file");
		
		lseek(src, 0, SEEK_SET);

		while ((read_bytes = read(src, buffer, BUFFER_SIZE))) {
			if (read_bytes < 0)
				fatal("Cannot read file");
			
			wrote_bytes = write(dest, buffer, read_bytes);
			if (wrote_bytes != read_bytes) 
				fatal("Cannot write to stdout");
		}
		close(src);
	}
	return 1;
}
