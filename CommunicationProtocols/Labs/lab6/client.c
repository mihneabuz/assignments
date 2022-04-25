/*
*  	Protocoale de comunicatii: 
*  	Laborator 6: UDP
*	client mini-server de backup fisiere
*/

#include <fcntl.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>

#include "helpers.h"

void usage(char*file)
{
	fprintf(stderr,"Usage: %s ip_server port_server file\n",file);
	exit(0);
}

/*
*	Utilizare: ./client ip_server port_server nume_fisier_trimis
*/
int main(int argc,char**argv)
{
	if (argc!=4)
		usage(argv[0]);
	
	int fd;
	struct sockaddr_in to_station;
	char buf[BUFLEN];

	/*Deschidere socket*/
	
	int s = socket(PF_INET, SOCK_DGRAM, 0);

	if (s < 0) {
		perror("Failed to create socket.\n");
		return 1;
	}
	
	/* Deschidere fisier pentru citire */
	DIE((fd=open(argv[3],O_RDONLY))==-1,"open file");
	
	/*Setare struct sockaddr_in pentru a specifica unde trimit datele*/
	
	to_station.sin_family = AF_INET;
	to_station.sin_port = htons((short)123456);
	inet_aton("127.0.0.1", &(to_station.sin_addr));	
	
	/*
	*  cat_timp  mai_pot_citi
	*		citeste din fisier
	*		trimite pe socket
	*/

	int read_bytes, sent_bytes;
	while ((read_bytes = read(fd, buf, BUFLEN))) {
		if (read_bytes < 0) {
			perror("Failed to read file.\n");
			return 1;
		}

		sent_bytes = sendto(s, buf, read_bytes, 0,
				(struct sockaddr*) &to_station, sizeof(to_station));
		if (sent_bytes != read_bytes) {
			perror("Failed to sent data.\n");
			return 1;
		}
	}

	buf[0] = 0;
	sendto(s, buf, 1, 0, (struct sockaddr*) &to_station, sizeof(to_station));

	/*Inchidere socket*/
	
	close(s);
	
	/*Inchidere fisier*/

	close(fd);

	return 0;
}
