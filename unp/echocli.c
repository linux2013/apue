#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#define SERV_PORT 8888

int
main(int argc, char * argv[])
{
    struct sockaddr_in servaddr;
    char buf[4096];
    char recvbuf[4096];
    int clifd;
    int n;

    if(argc != 2){
        printf("usage: %s <IPaddress>\n", argv[0]);
        exit(1);
    }
    
    if( (clifd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ){
        perror("socket:");
        exit(1);
    } 

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET, argv[1], &(servaddr.sin_addr)); 

    if(connect(clifd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0){
        perror("connect: ");
        exit(1);
    }
    
    while(fgets(buf, 4096, stdin) != NULL){
        if(write(clifd, buf, sizeof(buf)) < 0){
            perror("write:");
            exit(1);
        }
        
        if( (n = read(clifd, recvbuf,4096)) < 0){
            perror("read:");
            exit(1);
        }
        if( n == 0){
            printf("server terminated prematurely\n"); 
            exit(1);   
        }

        fputs(recvbuf, stdout);
    }
    
    exit(0);
}
