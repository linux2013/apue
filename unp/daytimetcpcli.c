#include <sys/socket.h>
#include <time.h>
#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#define MAXLINE 4096
#define LISTENQ 10

int
main(int argc, char* argv[])
{
    int sockfd, n;
    char recvline[MAXLINE];
    struct sockaddr_in seraddr;

    if(argc != 2){
        printf("usage: %s <IPaddress>\n", argv[0]);
        exit(1);
    }   

    if( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        printf("socket error\n");
        exit(1);
    } 

    bzero(&seraddr, sizeof(seraddr));
    seraddr.sin_family = AF_INET;
    seraddr.sin_port = htons(13);
    if(inet_pton(AF_INET, argv[1], &seraddr.sin_addr) < 0){
        printf("inet_pton error\n");
        exit(1);
    }

    if(connect(sockfd, (struct sockaddr *)&seraddr, sizeof(seraddr)) < 0){
        printf("connect error\n");
        perror("---");
        exit(1);
    }

    exit(0);
}


