#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"

void print_response(int id, char *response) {
    switch (id) {
        case 2:
            printf("Generally OK\n");
            break;
    
        case 3:
            printf("Resource has moved\n");
            break;

        case 4:
            printf("Something is wrong with you, the client\n");
            if (response[11] == '0') {
                printf("400 : Bad Request!\n");
            } 
            if (response[11] == '1') {
                printf("401 : Unauthorized!\n");
            }
            if (response[11] == '4') {
                printf("404 : Not Found!\n");
            }
            break;

        case 5:
            printf("Something is wrong with me, the server\n");
            break;
    }
    
    printf("%s", response);
    printf("\n");
}

int main(int argc, char *argv[])
{
    char *message;
    char *response;
    int sockfd;

    // Open the connection
    sockfd = open_connection("34.118.48.238", 8080, AF_INET, SOCK_STREAM, 0);

    // Ex 1.1: GET dummy from main server
    message = compute_get_request("34.118.48.238:8080", "/api/v1/dummy", NULL, NULL, 0);
    printf("%s", message);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    int id = response[9] - '0';
    print_response(id, response);
    printf("\n");

    char **data = (char **) malloc(100 * sizeof(char *));
    for (int i = 0; i < 100; i++)
        data[i] = (char *) malloc(100 * sizeof(char));

    strcpy(data[0], "Ana are mere!");
    strcpy(data[1], "Azi este o zi insorita.");
    strcpy(data[2], "1234567890");

    // Ex 1.2: POST dummy and print response from main server
    message = compute_post_request("34.118.48.238:8080", "/api/v1/dummy", "application/x-www-form-urlencoded", data, 3, NULL, 0);
    printf("%s", message);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    id = response[9] - '0';
    print_response(id, response);
    printf("\n");

    // Ex 2: Login into main server
    strcpy(data[0], "username=student&password=student");
    message = compute_post_request("34.118.48.238:8080", "/api/v1/auth/login", "application/x-www-form-urlencoded", data, 1, NULL, 0);
    printf("%s", message);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    printf("%s\n", response);
    id = response[9] - '0';
    print_response(id, response);
    printf("\n");

    // Ex 3: GET weather key from main server
    char **cookies = (char **) malloc(10 * sizeof(char *));
    for (int i = 0; i < 10; i++)
        cookies[i] = (char *) malloc(120 * sizeof(char));

    strcpy(cookies[0], "connect.sid=s%3AQNEdVhR-LDwDaCBQLme0xj-WvLBorVrc.VF98QqBHXepl%2BSM37cQYLr2kkV3sBxHTPpj%2BIJz0His");
    message = compute_get_request("34.118.48.238:8080", "/api/v1/weather/key", NULL, cookies, 1);
    printf("%s", message);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    id = response[9] - '0';
    print_response(id, response);
    printf("\n");

    // Ex 4: GET weather data from OpenWeather API
    message = compute_get_request("api.openweathermap.org", "/data/2.5/weather", "lat=35&lon=139&appid=b912dd495585fbf756dc6d8f415a7649", NULL, 0);
    printf("%s", message);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    id = response[9] - '0';
    print_response(id, response);
    printf("\n");

    // Ex 5: Logout from main server
    message = compute_get_request("34.118.48.238:8080", "/api/v1/auth/logout", NULL, NULL, 0);
    printf("%s", message);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    id = response[9] - '0';
    print_response(id, response);
    printf("\n");
    
    for (int i = 0; i < 10; i++)
        free(cookies[i]);
    free(cookies);

    return 0;
}
