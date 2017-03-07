#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER "127.0.0.1"
#define BUFLEN 512  /* Max length of buffer */
#define PORT 8888   /* The port on which to send data */

int main(void) {
    struct sockaddr_in server;
    int st;
    unsigned int slen;
    char buf[BUFLEN];
    char message[BUFLEN];

    slen = sizeof(server);

    if ( (st=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
        perror("socket");
        return EXIT_FAILURE;
    }

    memset((char *) &server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);

    if (inet_aton(SERVER , &server.sin_addr) == 0) {
        fprintf(stderr, "inet_aton() failed\n");
        return EXIT_FAILURE;
    }

    while(1) {
        printf("Enter message : ");
        fgets(message, BUFLEN, stdin);

        /* send the message */
        if (sendto(st, message, strlen(message) , 0 , (struct sockaddr *) &server, slen)==-1) {
          perror("sendto");
          return EXIT_FAILURE;
        }

        /* receive a reply and print it */
        /* clear the buffer by filling null, it might have previously received data */
        memset(buf,'\0', BUFLEN);
        /* try to receive some data, this is a blocking call */
        if (recvfrom(st, buf, BUFLEN, 0, (struct sockaddr *) &server, &slen) == -1) {
          perror("recvfrom");
          return EXIT_FAILURE;
        }
        puts(buf);
    }

    close(st);
    return 0;
}
