/*
 * Protocoale de comunicatii Laborator 11 - E-mail
 * send_mail.c
 */

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>  
#include <unistd.h>     
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#define SMTP_PORT 25
#define MAXLEN 500

/**
 * Citeste maxim maxlen octeti de pe socket-ul sockfd in
 * buffer-ul vptr. Intoarce numarul de octeti cititi.
 */
ssize_t read_line(int sockd, void *vptr, size_t maxlen)
{
    ssize_t n, rc;
    char c, *buffer;

    buffer = vptr;

    for (n = 1; n < maxlen; n++) {
        if ((rc = read(sockd, &c, 1)) == 1) {
            *buffer++ = c;

            if (c == '\n') {
                break;
            }
        } else if (rc == 0) {
            if (n == 1) {
                return 0;
            } else {
                break;
            }
        } else {
            if (errno == EINTR) {
                continue;
            }

            return -1;
        }
    }

    *buffer = 0;
    return n;
}

/**
 * Trimite o comanda SMTP si asteapta raspuns de la server. Comanda
 * trebuie sa fie in buffer-ul sendbuf. Sirul expected contine
 * inceputul raspunsului pe care trebuie sa-l trimita serverul
 * in caz de succes (de exemplu, codul 250). Daca raspunsul
 * semnaleaza o eroare, se iese din program.
 */
void send_command(int sockfd, const char sendbuf[], char *expected)
{
    char recvbuf[MAXLEN];
    int nbytes;
    char CRLF[2] = {13, 10};

    printf("Trimit: %s\n", sendbuf);
    write(sockfd, sendbuf, strlen(sendbuf));
    write(sockfd, CRLF, sizeof(CRLF));

    nbytes = read_line(sockfd, recvbuf, MAXLEN - 1);
    recvbuf[nbytes] = 0;
    printf("Am primit: %s", recvbuf);

    if (strstr(recvbuf, expected) != recvbuf) {
        printf("Am primit mesaj de eroare de la server!\n");
        exit(-1);
    }
}

int main(int argc, char **argv) {
    int sockfd;
    int port = SMTP_PORT;
    struct sockaddr_in servaddr;
    char server_ip[INET_ADDRSTRLEN];
    char sendbuf[MAXLEN]; 
    char recvbuf[MAXLEN];

    if (argc != 3) {
        printf("Utilizare: ./send_msg adresa_server nume_fisier\n");
        exit(-1);
    }

    strncpy(server_ip, argv[1], INET_ADDRSTRLEN);

    // TODO 1: creati socket-ul TCP client
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // TODO 2: completati structura sockaddr_in cu
    // portul si adresa IP ale serverului SMTP

    servaddr.sin_family = AF_INET;
    inet_aton(server_ip, &servaddr.sin_addr);
    servaddr.sin_port = htons(port);
        
    // TODO 3: conectati-va la server
    
    if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
        exit(1);

    // se primeste mesajul de conectare de la server
    read_line(sockfd, recvbuf, MAXLEN -1);
    printf("Am primit: %s\n", recvbuf);

    // se trimite comanda de HELO
    sprintf(sendbuf, "HELO client.upb.ro");
    send_command(sockfd, sendbuf, "250");

    // TODO 4: trimiteti comanda de MAIL FROM
    
    memset(sendbuf, 0, MAXLEN);
    sprintf(sendbuf, "MAIL FROM: <profesor@upb.ro>");
    send_command(sockfd, sendbuf, "250");

    // TODO 5: trimiteti comanda de RCPT TO

    memset(sendbuf, 0, MAXLEN);
    sprintf(sendbuf, "RCPT TO: <student@upb.ro>");
    send_command(sockfd, sendbuf, "250");

    // TODO 6: trimiteti comanda de DATA
    
    memset(sendbuf, 0, MAXLEN);
    sprintf(sendbuf, "DATA");
    send_command(sockfd, sendbuf, "354");

    // TODO 7: trimiteti e-mail-ul (antete + corp + atasament)

    FILE *f = fopen(argv[2], "r");
    char aux[MAXLEN];
    fread(aux, strlen(aux), sizeof(aux), f);

    memset(sendbuf, 0, MAXLEN);
    sprintf(sendbuf, "MIME-Version: 1.0\r\n"
            "From: Student Studentescu <student@upb.ro>\r\n"
            "To: Profesor PC <profesor@upb.ro>\r\n"
            "Subject: Re: Tema\r\n"
            "Content-Type: multipart/mixed; boundary=abc\r\n\r\n"
            "Content-Type: text/plain\r\n\r\n"
            "Atasez tema.\r\n\r\n"
            "Cu bine,\r\n"
            "Studentul\r\n\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Disposition: attachment; filename = \"file.txt\"\r\n\r\n"
            "%s\r\n"
            ".", aux);
    send_command(sockfd, sendbuf, "250");

    // TODO 8: trimiteti comanda de QUIT
    
    memset(sendbuf, 0, MAXLEN);
    sprintf(sendbuf, "QUIT");
    send_command(sockfd, sendbuf, "221");

    // TODO 9: inchideti socket-ul TCP client
    close(sockfd);

    return 0;
}