#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <signal.h>


#define SERV_PORT 8068


/*
 * this is the echo TCP server demo file
 */

static void sig_chld(int);

int
main(int argc, char * argv[])
{
    int listenfd, connfd;
    struct sockaddr_in servaddr;
    struct sockaddr_in cliaddr;
    socklen_t len;    
    pid_t pid;
    char str[4096];
    int n;
    struct sigaction act, oldact;


    if( (listenfd = socket(AF_INET, SOCK_STREAM,0)) < 0){  /* create an unbound TCP socket */
        //write(stdout, "Socket error\n", sizeof("Sokcet error\n"));
        perror("socket: ");
        exit(1);
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    len = sizeof(servaddr);
    if(bind(listenfd, (struct sockaddr *)&servaddr, len )== -1){ /* bind listen socket to local server */
        perror("bind: ");
        exit(1);
    }
    
    if(listen(listenfd, 10) < 0){ /* change listen socket file description into passive open*/
        perror("listen: ");
        exit(1);
    }

    act.sa_handler = sig_chld;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    if(sigaction(SIGCHLD, &act, &oldact) < 0){
        perror("sigaction: ");
        exit(1);
    }    

    while(1){
        AGAIN:
        len = sizeof(cliaddr);
        if( (connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &len)) < 0){ /*block process until tcp queue has cilent connection*/
            if(errno == EINTR){
                goto AGAIN;
            }
             perror("accept: ");
             exit(1);
        }

        if( (pid = fork()) < 0){ /* fork a child process, deal client request*/
            perror("fork: ");
            exit(1);
        }
    
        if(pid == 0){ /* child process */
            close(listenfd); /* close listen socket fd */
        
            DOAGAIN:
            while( (n = read(connfd, str, 4096)) > 0){
            
                 write(connfd, str, n); /* write data to client */
            }
        
            if((n < 0) && (errno = EINTR)){
                goto DOAGAIN;
            }else if(n < 0){
                perror("read: ");
                exit(1);
            }
        
            exit(0); /* child process exit */
        }
        close(connfd);
    }
}

static void sig_chld(int signo)
{
    pid_t pid;
    int stat; 
    while( (pid = waitpid(-1, &stat, WNOHANG)) < 0){
        printf("child %d terminated\n", pid);
    }
    return;
}
