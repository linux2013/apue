#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/select.h>
#include <math.h>

#define SERV_PORT 8068

int
main(int argc, char * argv[])
{
    struct sockaddr_in servaddr;
    char buf[4096];
    char recvbuf[4096];
    int clifd;
    int n, num;
    fd_set readfd;
    int maxfd1, stdineof;

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

/* use block I/O mode*/
/*    
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
*/  
    stdineof = 0;
    FD_ZERO(&readfd);  
    for(;;){
        if(0 == stdineof){
            FD_SET(fileno(stdin), &readfd);
        }       
        FD_SET(clifd, &readfd);
        maxfd1 = ((STDIN_FILENO > clifd)? STDIN_FILENO:clifd) + 1;

        if( (n = select(maxfd1, &readfd, NULL, NULL, NULL)) < 0){
            perror("select: ");
            exit(1);
        }

        if(FD_ISSET(clifd, &readfd)){
            if( (num = read(clifd, buf, 4096)) < 0){
                perror("read: ");
                exit(1);
            }
            if(0 == num){ /* read EOF */
                if(1 == stdineof){
                    return;
                }   
                printf("server terminated prematurely\n");
                exit(1);
            }
            if( write(STDOUT_FILENO, buf, num) < 0){
                perror("write: ");
                exit(1);
            
           } 
        }
        
        if(FD_ISSET(STDIN_FILENO, &readfd)){
            if((num = read(STDIN_FILENO, buf, 4096)) < 0){
                perror("read: ");
                exit(1);
            }
            if(0 == num){
                stdineof = 1;
                shutdown(clifd, SHUT_WR); /*send FIN */
                FD_CLR(STDIN_FILENO, &readfd);
                continue;
            }
    
            if(write(clifd, buf, num) < 0){
                perror("write: ");
                exit(1);
            }
        }
    }
    exit(0);
}
