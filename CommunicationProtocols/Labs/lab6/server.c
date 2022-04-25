/*
*  	Protocoale de comunicatii: 
*  	Laborator 6: UDP
*	mini-server de backup fisiere
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
	fprintf(stderr,"Usage: %s server_port file\n",file);
	exit(0);
}

/*
*	Utilizare: ./server server_port nume_fisier
*/
int main(int argc,char**argv)
{
	int fd;

	if (argc!=3)
		usage(argv[0]);
	
	struct sockaddr_in my_sockaddr, from_station ;
	char buf[BUFLEN];

	/*Deschidere socket*/

	int s = socket(PF_INET, SOCK_DGRAM, 0);

	if (s < 0) {
		perror("Failed to create socket.\n");
		return 1;
	}
	
	/*Setare struct sockaddr_in pentru a asculta pe portul respectiv */

	my_sockaddr.sin_family = AF_INET;
	my_sockaddr.sin_port = htons((short)123456);
	my_sockaddr.sin_addr.s_addr = INADDR_ANY;
	
	/* Legare proprietati de socket */

	int r = bind(s, (struct sockaddr*) &my_sockaddr, sizeof(my_sockaddr));
	
	if (r < 0) {
		perror("Failed to bind socket.\n");
		return 1;
	}
	
	/* Deschidere fisier pentru scriere */
	DIE((fd=open(argv[2],O_WRONLY|O_CREAT|O_TRUNC,0644))==-1,"open file");
	
	/*
	*  cat_timp  mai_pot_citi
	*		citeste din socket
	*		pune in fisier
	*/

	socklen_t length = sizeof(from_station);
	int rcv_bytes, wrote_bytes;
	while (1) {
		rcv_bytes = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr*) &from_station, &length);
		if (rcv_bytes == -1) {
			perror("Failed to read socket.\n");
			return 1;
		}

		if (rcv_bytes == 1 && buf[0] == 0)
			break;

		wrote_bytes = write(fd, buf, rcv_bytes);
		if (wrote_bytes != rcv_bytes) {
			perror("Failed to write file.\n");
			return 1;
		}
	}

	/*Inchidere socket*/	

	close(s);
	
	/*Inchidere fisier*/

	close(fd);
	return 0;
}
