#include <sys/socket.h>
#include <time.h>
#include <stdio.h>
#include <netdb.h>
#include <string.h>

#define MAXLINE 4096
#define LISTENQ 10

int
main(int argc, char* argv[])
{
    struct sockaddr_in servaddr;
    struct sockaddr_in cliaddr;
    socklen_t len;
    int listenfd;
    int connfd;
    char buf[MAXLINE];
    time_t ticks;


    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(13);

    bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    listen(listenfd, LISTENQ);

    for(;;)
    {
        len = sizeof(cliaddr);

        connfd = accept(listenfd, (struct sockaddr *)&cliaddr,&len );

        printf("connettion form %s, port %d\n", inet_ntop(AF_INET, &cliaddr.sin_addr, buf, sizeof(buf)), ntohs(cliaddr.sin_port));
        close(connfd);
    }
    
}


