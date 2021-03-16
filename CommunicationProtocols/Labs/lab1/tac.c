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
	
	int i, offset, count, test;
	char buffer[BUFFER_SIZE], c;

	for (i = 1; i < argc; i++) {
		src = open(argv[i], O_RDONLY);
		if (src < 0)
			fatal("Cannot open file");
		
		offset = lseek(src, -2, SEEK_END);
		count = 0;

		while (offset > 0) {
			test = read(src, &c, 1);
			if (test < 0)
				fatal("Cannot read from file");
			count += 1;

			if (c == '\n') {
				read(src, buffer, count);
				test = write(dest, buffer, count);
				if (test < 0)
					fatal("Cannot write to stdout");
				lseek(src, -count, SEEK_CUR);
				count = 0;
			}

			offset = lseek(src, -2, SEEK_CUR);
		}
		read(src, buffer, count + 1);
		test = write(dest, buffer, count + 1);
		if (test < 0)
			fatal("Cannot write to stdout");
		write(dest, "\n", 1);
		close(src);
	}
	return 1;
}
